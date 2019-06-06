#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_i2c.h>
#include <stm32f1xx_ll_gpio.h>

#include "I2CDriver.h"

I2C_HandleTypeDef handle;

void initI2C()
{
	// Enable I2C
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

	// Initialize I2C pins
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6 , LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7 , LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);

	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4);


	// Initialie I2C
	handle.Instance = I2C1;
	handle.Init.ClockSpeed = 100000;
	handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle.Init.OwnAddress1 = 0;
	handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	handle.Init.OwnAddress2 = 0;
	handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&handle);
}

uint8_t readByteI2C(uint8_t devaddr, uint8_t memaddr)
{
	uint8_t val;
	HAL_I2C_Mem_Read(&handle, devaddr, memaddr, 1, &val, 1, 1);
	return val;
}
