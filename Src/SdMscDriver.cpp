#include <Arduino_FreeRTOS.h>

#include "SdMscDriver.h"
#include "SdCard/SdSpiCard.h"

#include "SdFatSPIDriver.h"

#include "USBDebugLogger.h"

extern SdFatSPIDriver spiDriver;
SdSpiCard card;


volatile bool bReady = false;

enum IOOperation
{
	IO_Read,
	IO_Write
};

struct IOMsg
{
	IOOperation op;
	uint32_t lba;
	uint8_t * buf;
	uint16_t len;
};

// A queue of IO commands to execute in a separate thread and command results
QueueHandle_t sdCmdQueue = NULL;


// Initialize thread responsible for communication with SD card
bool initSDIOThread()
{
	// Initialize queues
	sdCmdQueue = xQueueCreate(1, sizeof(IOMsg));

	bool res = card.begin(&spiDriver, PA4, SPI_FULL_SPEED);
	return res;
}


const uint8_t SD_MSC_Inquirydata[] = {/* 36 */
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                     /* Version      : 4 Bytes */
};

int8_t SD_MSC_Init (uint8_t lun)
{
	(void)lun; // Not used

//	if(!card.begin(&spiDriver, 0, SPI_FULL_SPEED))
//		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_GetCapacity (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
	(void)lun; // Not used

	*block_num  = card.cardSize();
	*block_size = 512;
	return (USBD_OK);
}

int8_t  SD_MSC_IsReady (uint8_t lun)
{
	(void)lun; // Not used

	return bReady ? USBD_OK : USBD_FAIL;
}

int8_t  SD_MSC_IsWriteProtected (uint8_t lun)
{
	(void)lun; // Not used

	return (USBD_OK); // Never write protected
}

int8_t SD_MSC_Read (uint8_t lun,
						uint8_t *buf,
						uint32_t blk_addr,
						uint16_t blk_len)
{
	// Send read command to IO executor thread
	IOMsg msg;
	msg.op = IO_Read;
	msg.lba = blk_addr;
	msg.len = blk_len;
	msg.buf = buf;

	if(xQueueSendFromISR(sdCmdQueue, &msg, NULL) != pdPASS)
		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_Write (uint8_t lun,
						 uint8_t *buf,
						 uint32_t blk_addr,
						 uint16_t blk_len)
{
	return USBD_FAIL;

	// Send read command to IO executor thread
	IOMsg msg;
	msg.op = IO_Write;
	msg.lba = blk_addr;
	msg.len = blk_len;
	msg.buf = buf;

	if(xQueueSendFromISR(sdCmdQueue, &msg, NULL) != pdPASS)
		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_GetMaxLun (void)
{
  return 0; // We have just 1 Logic unit number (LUN) which is zero
}

USBD_StorageTypeDef SdMscDriver =
{
	SD_MSC_Init,
	SD_MSC_GetCapacity,
	SD_MSC_IsReady,
	SD_MSC_IsWriteProtected,
	SD_MSC_Read,
	SD_MSC_Write,
	SD_MSC_GetMaxLun,
	(int8_t *)SD_MSC_Inquirydata,
};


extern "C" void cardReadCompletedCB(uint8_t res);
extern "C" void cardWriteCompletedCB(uint8_t res);

void xSDIOThread(void *pvParameters)
{
	vTaskDelay(3000);
	//serialDebugWrite("   SD : Listening for SD commands\r\n");

	//enable clock to the GPIOC peripheral
	__HAL_RCC_GPIOB_IS_CLK_ENABLED();

	uint32_t pin = LL_GPIO_PIN_10;
	LL_GPIO_SetPinMode(GPIOB, pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOB, pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, pin, LL_GPIO_SPEED_FREQ_HIGH);
	pin = LL_GPIO_PIN_11;
	LL_GPIO_SetPinMode(GPIOB, pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOB, pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, pin, LL_GPIO_SPEED_FREQ_HIGH);

	bReady = true;

	while(true)
	{
		IOMsg msg;
		if(xQueueReceive(sdCmdQueue, &msg, portMAX_DELAY))
		{
			//serialDebugWrite("   SD : Received IO message %d for lba %d\r\n", msg.op, msg.lba);

			LL_GPIO_SetOutputPin(GPIOB, pin);

			switch(msg.op)
			{
				case IO_Read:
				{
					bool res = card.readBlocks(msg.lba, msg.buf, msg.len);
					cardReadCompletedCB(res ? 0 : 0xff);
					break;
				}
				case IO_Write:
				{
					bool res = card.writeBlocks(msg.lba, msg.buf, msg.len);
					cardWriteCompletedCB(res? 0 : 0xff);
					break;
				}
				default:
					break;
			}

			LL_GPIO_ResetOutputPin(GPIOB, pin);

			//serialDebugWrite("   SD : Operation finished for LBA %d. Sending result back\r\n", msg.lba);
		}
	}

}

/*
uint8_t io_buf[1024];
static TaskHandle_t xTestTask = NULL;

extern "C" void cardReadCompletedCB(uint8_t res)
{
	xTaskNotifyGive(xTestTask);
}

extern "C" void cardWriteCompletedCB(uint8_t res)
{
	xTaskNotifyGive(xTestTask);
}

void xSDTestThread(void *pvParameters)
{
	xTestTask = xTaskGetCurrentTaskHandle();

	vTaskDelay(3500);
	usbDebugWrite("Reading SD card\r\n");

	uint32_t prev = HAL_GetTick();
	uint32_t opsPer1s = 0;
	uint32_t cardSize = card.cardSize();
	for(uint32_t i=0; i<cardSize; i++)
	{
		opsPer1s++;

		//serialDebugWrite("Reading block %d\r\n", i);

		if(SD_MSC_Read(0, io_buf, i, 2) != 0)
			usbDebugWrite("Failed to read block %d\r\n", i);
		//else
		//	serialDebugWrite("Successfully read block %d\r\n", i);

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if(HAL_GetTick() - prev > 1000)
		{
			prev = HAL_GetTick();
			usbDebugWrite("Reading speed: %d kbytes/s\r\n", opsPer1s);
			opsPer1s = 0;
		}
	}

	while(true)
		;
}

*/
