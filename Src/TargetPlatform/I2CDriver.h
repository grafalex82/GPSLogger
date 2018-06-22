#ifndef I2CDRIVER_H
#define I2CDRIVER_H

#include <stdint.h>

#ifdef STM32F1
	#include <stm32f1xx_hal_dma.h>
	#include <stm32f1xx_hal_i2c.h>
#elif STM32F4
	#include <stm32f4xx_hal_dma.h>
	#include <stm32f4xx_hal_i2c.h>
#endif //STM32F1

class I2CDriver
{
	// I2C device handlers
	I2C_HandleTypeDef handle;
	DMA_HandleTypeDef hdma_tx;

public:
	I2CDriver();

	bool init();
	bool writeMem(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);
	bool writeMemDMA(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);

	DMA_HandleTypeDef * getDMAHandle();
	void transferCompletedCB();
};

extern I2CDriver i2cDriver;

#endif // I2CDRIVER_H
