#ifndef SPIDISPLAYDRIVER_H
#define SPIDISPLAYDRIVER_H

#include "FreeRTOSHelpers.h"

#include <Adafruit_SSD1306.h>
#include <Arduino_FreeRTOS.h>

#include <stm32f1xx_hal_spi.h>
#include <stm32f1xx_hal_dma.h>

class SPIDisplayDriver: public ISSD1306Driver
{
	// Device handles
	SPI_HandleTypeDef spiHandle;
	DMA_HandleTypeDef dmaHandleTx;

	// transfer synchronization object
	Sema sema;

public:
	SPIDisplayDriver();

	virtual void begin();
	virtual void startTransaction();
	virtual void sendCommand(uint8_t cmd);
	virtual void sendCommands(const uint8_t *cmds, size_t size);
	virtual void sendData(const uint8_t * data, size_t size);
	virtual void endTransaction();

	// Handle getter
	SPI_HandleTypeDef getHandle() const {return spiHandle;}

	// DMA transter callback
	void dmaTransferCompletedCB();
};

#endif // SPIDISPLAYDRIVER_H
