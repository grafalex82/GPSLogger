#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_i2c.h>

#include "DisplayDriver.h"

DisplayDriver::DisplayDriver(int8_t i2caddr)
{
	i2c_addr = i2caddr << 1; // account LSB for R/W flag
}

void DisplayDriver::begin()
{
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
}

void DisplayDriver::sendCommand(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&handle, i2c_addr, 0x00, 1, &cmd, 1, 10);
}

void DisplayDriver::sendData(uint8_t * data, size_t size)
{
	HAL_I2C_Mem_Write(&handle, i2c_addr, 0x40, 1, data, size, 10);
}
