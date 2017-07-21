#include <usbd_cdc_if.h>
#include <usbd_cdc.h>
#include <usbd_msc.h>
#include <usbd_desc.h>

#include <stm32f1xx_ll_gpio.h>

#include <string.h> // strlen
#include <stdarg.h> // VA

#include <Arduino_FreeRTOS.h>
#include "FreeRTOSHelpers.h"

#include "USBDebugLogger.h"

#include "SdMscDriver.h"

#define USB_SERIAL_BUFFER_SIZE 256

uint8_t usbTxBuffer[USB_SERIAL_BUFFER_SIZE];
volatile uint16_t usbTxHead = 0;
volatile uint16_t usbTxTail = 0;
volatile uint16_t usbTransmitting = 0;

extern USBD_HandleTypeDef hUsbDeviceFS;

// TODO Make it static
SemaphoreHandle_t usbMutex = NULL;

USBD_HandleTypeDef hUsbDeviceFS;

void reenumerateUSB()
{
	// Initialize PA12 pin
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW);

	// Let host know to enumerate USB devices on the bus
	LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_12);
	HAL_Delay(1);

	// Restore pin mode
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_FLOATING);
	HAL_Delay(1);
}

void initUSB()
{
	reenumerateUSB();

	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC);
	//USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_MSC_RegisterStorage(&hUsbDeviceFS, &SdMscDriver);
	USBD_Start(&hUsbDeviceFS);

	usbMutex = xSemaphoreCreateMutex();
}

extern PCD_HandleTypeDef hpcd_USB_FS;

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

void usbDebugWriteInternal(const char *buffer, size_t size, bool reverse = false)
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

		usbTxBuffer[usbTxHead] = *buffer;
		usbTxHead = (usbTxHead + 1) % sizeof(usbTxBuffer);

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

// sprintf implementation takes more than 10kb and adding heap to the project. I think this is
// too much for the functionality I need
//
// Below is a homebrew printf-like dumping function which accepts:
// - %d for digits
// - %x for numbers as HEX
// - %s for strings
// - %% for percent symbol
//
// Implementation supports also value width as well as zero padding

// Print the number to the buffer (in reverse order)
// Returns number of printed symbols
size_t PrintNum(unsigned int value, uint8_t radix, char * buf, uint8_t width, char padSymbol)
{
	//TODO check negative here

	size_t len = 0;

	// Print the number
	do
	{
		char digit = value % radix;
		*(buf++) = digit < 10 ? '0' + digit : 'A' - 10 + digit;
		value /= radix;
		len++;
	}
	while (value > 0);

	// Add zero padding
	while(len < width)
	{
		*(buf++) = padSymbol;
		len++;
	}

	return len;
}

void usbDebugWrite(const char * fmt, ...)
{
	va_list v;
	va_start(v, fmt);

	const char * chunkStart = fmt;
	size_t chunkSize = 0;

	char ch;
	do
	{
		// Get the next byte
		ch = *(fmt++);

		// Just copy the regular characters
		if(ch != '%')
		{
			chunkSize++;
			continue;
		}

		// We hit a special symbol. Dump string that we processed so far
		if(chunkSize)
			usbDebugWriteInternal(chunkStart, chunkSize);

		// Process special symbols

		// Check if zero padding requested
		char padSymbol = ' ';
		ch = *(fmt++);
		if(ch == '0')
		{
			padSymbol = '0';
			ch = *(fmt++);
		}

		// Check if width specified
		uint8_t width = 0;
		if(ch > '0' && ch <= '9')
		{
			width = ch - '0';
			ch = *(fmt++);
		}

		// check the format
		switch(ch)
		{
			case 'd':
			case 'u':
			{
				char buf[12];
				size_t len = PrintNum(va_arg(v, int), 10, buf, width, padSymbol);
				usbDebugWriteInternal(buf + len, len, true);
				break;
			}
			case 'x':
			case 'X':
			{
				char buf[9];
				size_t len = PrintNum(va_arg(v, int), 16, buf, width, padSymbol);
				usbDebugWriteInternal(buf + len, len, true);
				break;
			}
			case 's':
			{
				char * str = va_arg(v, char*);
				usbDebugWriteInternal(str, strlen(str));
				break;
			}
			case '%':
			{
				usbDebugWriteInternal(fmt-1, 1);
				break;
			}
			default:
				// Otherwise store it like a regular symbol as a part of next chunk
				fmt--;
				break;
		}

		chunkStart = fmt;
		chunkSize=0;
	}
	while(ch != 0);

	if(chunkSize)
		usbDebugWriteInternal(chunkStart, chunkSize - 1); // Not including terminating NULL

	va_end(v);
}

void usbDebugWrite(char c)
{
	usbDebugWriteInternal(&c, 1);
}

