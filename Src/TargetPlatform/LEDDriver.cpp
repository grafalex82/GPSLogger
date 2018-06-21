#ifdef STM32F1
	#include <stm32f1xx_hal.h>
	#include <stm32f1xx_hal_rcc.h>
	#include <stm32f1xx_ll_gpio.h>
#elif STM32F4
	#include <stm32f4xx_hal.h>
	#include <stm32f4xx_ll_gpio.h>
#endif

#include "LEDDriver.h"

#ifdef STM32F1
	const uint32_t LEDDriver::pin = LL_GPIO_PIN_13;
	#define LED_PORT GPIOC
	#define ENABLE_GPIO_CLOCK __HAL_RCC_GPIOC_CLK_ENABLE
#elif STM32F4
	const uint32_t LEDDriver::pin = LL_GPIO_PIN_6;
	#define LED_PORT GPIOA
	#define ENABLE_GPIO_CLOCK __HAL_RCC_GPIOA_CLK_ENABLE
#endif

void LEDDriver::init()
{
	if(inited)
		return;

	//enable clock to the GPIO peripheral
	ENABLE_GPIO_CLOCK();

	// Init LED pins as output
	LL_GPIO_SetPinMode(LED_PORT, pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(LED_PORT, pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(LED_PORT, pin, LL_GPIO_SPEED_FREQ_LOW);

	inited = true;
}

void LEDDriver::turnOn()
{
	LL_GPIO_ResetOutputPin(LED_PORT, pin);
}

void LEDDriver::turnOff()
{
	LL_GPIO_SetOutputPin(LED_PORT, pin);
}

void LEDDriver::toggle()
{
	LL_GPIO_TogglePin(LED_PORT, pin);
}
