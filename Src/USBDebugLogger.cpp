#include <usbd_cdc_if.h>
//#include <usbd_cdc.h>
#include <usbd_msc_cdc.h>
//#include <usbd_msc.h>
#include <usbd_desc.h>

#include <stm32f1xx_ll_gpio.h>

#include <string.h> // strlen
#include <stdarg.h> // VA

#include <Arduino_FreeRTOS.h>
#include "FreeRTOSHelpers.h"

#include "USBDebugLogger.h"
#include "PrintUtils.h"

#include "SdMscDriver.h"
#include "LEDThread.h"

#define USB_SERIAL_BUFFER_SIZE 256

uint8_t usbTxBuffer[USB_SERIAL_BUFFER_SIZE];
volatile uint16_t usbTxHead = 0;
volatile uint16_t usbTxTail = 0;
volatile uint16_t usbTransmitting = 0;

// TODO Make it static
SemaphoreHandle_t usbMutex = NULL;

USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

void reenumerateUSB()
{
	// Initialize PA12 pin
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW);

	// Let host know to enumerate USB devices on the bus
	LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_12);
	HAL_Delay(200);

}

void initUSB()
{
	HAL_Delay(2000);
	blink(1);
	//HAL_PCD_Stop(&hpcd_USB_FS);

	reenumerateUSB();

	HAL_Delay(1000);
	blink(2);

	USBD_StatusTypeDef res = USBD_Init(&hUsbDeviceFS, &FS_Desc, 0);
	if(res)
		halt(res);

	halt(6);

	HAL_Delay(1000);
	blink(3);

	HAL_Delay(2);

	res = USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC_CDC_ClassDriver);
	if(res)
		halt(res);

	HAL_Delay(1000);
	blink(4);

	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_MSC_RegisterStorage(&hUsbDeviceFS, &SdMscDriver);

	HAL_Delay(1000);
	blink(5);

	USBD_Start(&hUsbDeviceFS);

	HAL_Delay(1000);
	blink(6);


	// Restore pin mode
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_FLOATING);
	HAL_Delay(200);

	usbMutex = xSemaphoreCreateMutex();
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void) {
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

uint16_t transmitContiguousBuffer()
{
	uint16_t count = 0;

	// Transmit the contiguous data up to the end of the buffer
	if (usbTxHead > usbTxTail)
	{
		count = usbTxHead - usbTxTail;
	}
	else
	{
		count = sizeof(usbTxBuffer) - usbTxTail;
	}

	CDC_Transmit_FS(&usbTxBuffer[usbTxTail], count);

	return count;
}

struct USBTarget: public PrintTarget
{
	virtual void operator()(char c)
	{
		USBTarget::operator()(&c, 1);
	}

	virtual void operator()(const char *buffer, size_t size, bool reverse = false)
	{
		// Ignore sending the message if USB is not connected
		if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
			return;

		// Transmit the message but no longer than timeout
		uint32_t timeout = HAL_GetTick() + 5;

		// Protect this function from multiple entrance
		MutexLocker locker(usbMutex);

		// Copy data to the buffer
		for(size_t i=0; i < size; i++)
		{
			if(reverse)
				--buffer;

			if(*buffer) // Skip zeroes (including terminating zeroes)
			{
				usbTxBuffer[usbTxHead] = *buffer;
				usbTxHead = (usbTxHead + 1) % sizeof(usbTxBuffer);
			}

			if(!reverse)
				buffer++;

			// Wait until there is a room in the buffer, or drop on timeout
			while(usbTxHead == usbTxTail && HAL_GetTick() < timeout);
			if (usbTxHead == usbTxTail) break;
		}

		// If there is no transmittion happening
		if (usbTransmitting == 0)
		{
			usbTransmitting = transmitContiguousBuffer();
		}
	}
};

extern "C" void USBSerialTransferCompletedCB()
{
	usbTxTail = (usbTxTail + usbTransmitting) % sizeof(usbTxBuffer);

	if (usbTxHead != usbTxTail)
	{
		usbTransmitting = transmitContiguousBuffer();
	}
	else
	{
		usbTransmitting = 0;
	}
}

void usbDebugWrite(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	USBTarget target;
	print(target, fmt, args);
	va_end(args);
}

void usbDebugWrite(char c)
{
	USBTarget target;
	target(c);
}

