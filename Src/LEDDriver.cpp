#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_gpio.h>

#include "LEDDriver.h"

#define LED_PORT GPIOC
const uint16_t LED_PIN = GPIO_PIN_13;

LEDDriver::LEDDriver()
{
	// enable clock to GPIOC
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// Turn off the LED by default
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

	// Initialize PC13 pin
	GPIO_InitTypeDef ledPinInit;
	ledPinInit.Pin = LED_PIN;
	ledPinInit.Mode = GPIO_MODE_OUTPUT_PP;
	ledPinInit.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &ledPinInit);
}

void LEDDriver::turnOn()
{
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void LEDDriver::turnOff()
{
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void LEDDriver::toggle()
{
	HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
