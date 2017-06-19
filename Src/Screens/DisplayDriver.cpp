#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_i2c.h>

#include "DisplayDriver.h"

// A display driver instance
DisplayDriver displayDriver;

DisplayDriver::DisplayDriver(int8_t i2caddr)
{
	i2c_addr = i2caddr << 1; // account LSB for R/W flag
	xDisplayThread = NULL;
}

void DisplayDriver::begin()
{
	// Init sync object
	xDisplayThread = xTaskGetCurrentTaskHandle();

	// Initialize I2C pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Enable I2C
	__I2C1_CLK_ENABLE();

	// Initialie I2C
	handle.Instance = I2C1;
	handle.Init.ClockSpeed = 400000;
	handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle.Init.OwnAddress1 = 0;
	handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	handle.Init.OwnAddress2 = 0;
	handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	HAL_I2C_Init(&handle);

	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Initialize DMA
	hdma_tx.Instance                 = DMA1_Channel6;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	HAL_DMA_Init(&hdma_tx);

	// Associate the initialized DMA handle to the the I2C handle
	__HAL_LINKDMA(&handle, hdmatx, hdma_tx);

	/* DMA interrupt init */
	/* DMA1_Channel6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void DisplayDriver::sendCommand(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&handle, i2c_addr, 0x00, 1, &cmd, 1, 10);
}

void DisplayDriver::sendData(uint8_t * data, size_t size)
{
	// Start data transfer
	HAL_I2C_Mem_Write_DMA(&handle, i2c_addr, 0x40, 1, data, size);

	// Wait until transfer is completed
	ulTaskNotifyTake(pdTRUE, 100);
}

DMA_HandleTypeDef * DisplayDriver::getDMAHandle()
{
	return &hdma_tx;
}

void DisplayDriver::transferCompletedCB()
{
	// Resume display thread
	vTaskNotifyGiveFromISR(xDisplayThread, NULL);
}

extern "C" void DMA1_Channel6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(displayDriver.getDMAHandle());
}

extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	displayDriver.transferCompletedCB();
}
