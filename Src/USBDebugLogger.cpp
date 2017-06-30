#include <usbd_cdc_if.h>
#include <usbd_cdc.h>

#include <Arduino_FreeRTOS.h>
#include "FreeRTOSHelpers.h"
#include "USBDebugLogger.h"


#define USB_SERIAL_BUFFER_SIZE 128

uint8_t usbTxBuffer[CDC_SERIAL_BUFFER_SIZE];
volatile uint16_t usbTxHead = 0;
volatile uint16_t usbTxTail = 0;
volatile uint16_t usbTransmitting = 0;

extern USBD_HandleTypeDef hUsbDeviceFS;

// TODO Make it static
SemaphoreHandle_t usbMutex = NULL;

void initUsbDebugLogger()
{
	usbMutex = xSemaphoreCreateMutex();
}

void usbDebugWrite(uint8_t c)
{
	usbDebugWrite(&c, 1);
}

void usbDebugWrite(const char * str)
{
	usbDebugWrite((const uint8_t *)str, strlen(str));
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

void usbDebugWrite(const uint8_t *buffer, size_t size)
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
		usbTxBuffer[usbTxHead] = *buffer;
		usbTxHead = (usbTxHead + 1) % sizeof(usbTxBuffer);
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
