#ifndef SDIODRIVER_H
#define SDIODRIVER_H

#include <Arduino_FreeRTOS.h>

#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_sd.h>

typedef void * SemaphoreHandle_t;

class SDIODriver
{
	// SDIO stuff
	SD_HandleTypeDef sdHandle;
	DMA_HandleTypeDef dmaReadHandle;
	DMA_HandleTypeDef dmaWriteHandle;

	DMA_HandleTypeDef * curDMAHandle;

	// Synchronization semaphore handle
	SemaphoreHandle_t xSema = NULL;
	StaticSemaphore_t xSemaBuffer;

public:
	SDIODriver();

	bool init();

	bool cardRead(uint32_t lba, uint8_t * pBuf, uint32_t blocksCount);
	bool cardWrite(uint32_t lba, const uint8_t * pBuf, uint32_t blocksCount);

	// DMA Handle getter
	DMA_HandleTypeDef * getDMAHandle() {return curDMAHandle;}
	SD_HandleTypeDef * getHandle() {return &sdHandle;}

	// DMA transter callback
	void dmaTransferCompletedCB();

private:
	void initSDIOPins();
	bool initSDIO();
	bool initCard();
};

#endif // SDIODRIVER_H
