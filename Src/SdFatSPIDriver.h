#ifndef SDFATSPIDRIVER_H
#define SDFATSPIDRIVER_H

#include "SpiDriver/SdSpiBaseDriver.h"

#include <stm32f1xx_hal_spi.h>

//Forward declarations
class SPISettings;
typedef void * SemaphoreHandle_t;

// This is custom implementation of SPI Driver class. SdFat library is
// using this class to access SD card over SPI
//
// Main intention of this implementation is to drive data transfer
// over DMA and synchronize with FreeRTOS capabilities.

class SdFatSPIDriver : public SdSpiBaseDriver
{
	// SPI module
	SPI_HandleTypeDef spiHandle;
	DMA_HandleTypeDef dmaHandleRx;
	DMA_HandleTypeDef dmaHandleTx;

	// Synchronization semaphore handle
	SemaphoreHandle_t xSema = NULL;

public:
	SdFatSPIDriver();

	virtual void activate();
	virtual void begin(uint8_t chipSelectPin);
	virtual void deactivate();
	virtual uint8_t receive();
	virtual uint8_t receive(uint8_t* buf, size_t n);
	virtual void send(uint8_t data);
	virtual void send(const uint8_t* buf, size_t n);
	virtual void select();
	virtual void setSpiSettings(const SPISettings & spiSettings);
	virtual void unselect();

	// Handle getter
	SPI_HandleTypeDef getHandle() const {return spiHandle;}

	// DMA transter callback
	void dmaTransferCompletedCB();
};


#endif //SDFATSPIDRIVER_H
