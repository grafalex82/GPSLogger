#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_gpio.h>

#include "LEDThread.h"

#include <FreeRTOS.h>
#include <task.h>

#include "usbd_cdc_if.h"

#define LED_PORT GPIOC
const uint16_t LED_PIN = GPIO_PIN_13;

// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
public:
	LEDDriver()
	{
		// enable clock to GPIOC
		__HAL_RCC_GPIOC_CLK_ENABLE();

		// Turn off the LED by default
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

		// Initialize PC13 pin
		GPIO_InitTypeDef ledPinInit;
		ledPinInit.Pin = LED_PIN;
		ledPinInit.Mode = GPIO_MODE_OUTPUT_PP;
		ledPinInit.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(LED_PORT, &ledPinInit);
	}

	void turnOn()
	{
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
	}

	void turnOff()
	{
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
	}

	void toggle()
	{
		HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
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

		CDC_Transmit_FS((uint8_t*)"Ping\n", 5);
	}
}
