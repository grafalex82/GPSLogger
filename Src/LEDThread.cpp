#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_ll_gpio.h>

#include "LEDThread.h"
#include <Arduino_FreeRTOS.h>


// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
	const uint32_t pin = LL_GPIO_PIN_13;
public:
	LEDDriver()
	{
		//enable clock to the GPIOC peripheral
		__HAL_RCC_GPIOC_IS_CLK_ENABLED();

		// Init PC 13 as output
		LL_GPIO_SetPinMode(GPIOC, pin, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(GPIOC, pin, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(GPIOC, pin, LL_GPIO_SPEED_FREQ_LOW);
	}

	void turnOn()
	{
		LL_GPIO_ResetOutputPin(GPIOC, pin);
	}

	void turnOff()
	{
		LL_GPIO_SetOutputPin(GPIOC, pin);
	}

	void toggle()
	{
		LL_GPIO_TogglePin(GPIOC, pin);
	}
};


void vLEDThread(void *pvParameters)
{
	LEDDriver led;

	// Just blink once in 2 seconds
	for (;;)
	{
		vTaskDelay(2000);
		led.turnOn();
		vTaskDelay(100);
		led.turnOff();
	}
}
