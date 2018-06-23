#ifdef STM32F1
	#include <stm32f1xx_hal_rcc.h>
	#include <stm32f1xx_hal_cortex.h>
	#include <stm32f1xx_ll_gpio.h>
#elif STM32F4
	#include <stm32f4xx_hal_rcc.h>
	#include <stm32f4xx_hal_cortex.h>
	#include <stm32f4xx_hal_gpio.h>

	#define DMA1_Channel6 DMA1_Stream6
	#define DMA1_Channel6_IRQn DMA1_Stream6_IRQn
#endif //STM32F1

#include "I2CDriver.h"

// An I2C driver instance
I2CDriver i2cDriver;

I2CDriver::I2CDriver()
{
	xDMATransferThread = NULL;
}

bool I2CDriver::init()
{
	// Enable clock for GPIOB where I2C pins are located
	__HAL_RCC_GPIOB_CLK_ENABLE();
	// Enable I2C
	__HAL_RCC_I2C1_CLK_ENABLE();

	// Initialize I2C pins
	//LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6 , LL_GPIO_MODE_ALTERNATE);
	//LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
	//LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7 , LL_GPIO_MODE_ALTERNATE);
	//LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Initialie I2C
	handle.Instance = I2C1;
	handle.Init.ClockSpeed = 400000;
	handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle.Init.OwnAddress1 = 0;
	handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	handle.Init.OwnAddress2 = 0;
	handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&handle);

	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Initialize DMA
	hdma_tx.Instance                 = DMA1_Channel6;
	hdma_tx.Init.Channel 			 = DMA_CHANNEL_1;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_MEDIUM;
	hdma_tx.Init.FIFOMode			 = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_tx);

	// Associate the initialized DMA handle to the the I2C handle
	__HAL_LINKDMA(&handle, hdmatx, hdma_tx);

	// DMA interrupt init
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

	return true;
}

bool I2CDriver::writeMem(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size)
{
	// TODO Add mutex here

	return HAL_I2C_Mem_Write(&handle, devAddr, memAddr, 1, pData, size, 20) == HAL_OK;
}

bool I2CDriver::writeMemDMA(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size)
{
	// TODO Add mutex here

	xDMATransferThread = xTaskGetCurrentTaskHandle();
	if(HAL_I2C_Mem_Write_DMA(&handle, devAddr, memAddr, 1, pData, size) != HAL_OK)
		return false;
\
	// Wait until transfer is completed
	ulTaskNotifyTake(pdTRUE, 100);
	xDMATransferThread = NULL;
	return true;
}

DMA_HandleTypeDef * I2CDriver::getDMAHandle()
{
	return &hdma_tx;
}

void I2CDriver::transferCompletedCB()
{
	// Resume display thread
	vTaskNotifyGiveFromISR(xDMATransferThread, NULL);
}

#ifdef STM32F1
extern "C" void DMA1_Channel6_IRQHandler()
#elif STM32F4
extern "C" void DMA1_Stream6_IRQHandler()
#endif
{
	HAL_DMA_IRQHandler(i2cDriver.getDMAHandle());
}

extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	i2cDriver.transferCompletedCB();
}

