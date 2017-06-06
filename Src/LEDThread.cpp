#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_gpio.h>

#include "LEDThread.h"

#include <Arduino_FreeRTOS.h>

#include "SerialUSB.h"


// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
public:
	LEDDriver()
	{
		pinMode(PC13, OUTPUT);
	}

	void turnOn()
	{
		digitalWrite(PC13, LOW);
	}

	void turnOff()
	{
		digitalWrite(PC13, HIGH);
	}

	void toggle()
	{
		digitalWrite(PC13, !digitalRead(PC13));
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
