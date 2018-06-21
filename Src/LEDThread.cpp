#include "LEDThread.h"
#include "TargetPlatform\LEDDriver.h"
#include <Arduino_FreeRTOS.h>
#include "USBDebugLogger.h"
#include "SerialDebugLogger.h"

LEDDriver led;

volatile uint8_t ledStatus = 0xff;

void blink(uint8_t status)
{
	led.init();

	for(int i=0; i<3; i++)
	{
		led.turnOn();
		if(status & 0x4)
			HAL_Delay(300);
		else
			HAL_Delay(100);
		led.turnOff();

		status <<= 1;

		HAL_Delay(200);
	}
}

void setLedStatus(uint8_t status)
{
	ledStatus = status;
}

void halt(uint8_t status)
{
	led.init();

	while(true)
	{
		blink(status);

		HAL_Delay(700);
	}
}

void vLEDThread(void *pvParameters)
{
	led.init();

	// Just blink once in 2 seconds
	for (;;)
	{
		vTaskDelay(2000);

		if(ledStatus == 0xff)
		{
			led.turnOn();
			vTaskDelay(100);
			led.turnOff();
		}
		else
		{
			blink(ledStatus);
		}

		//usbDebugWrite("Test\n");
		//serialDebugWrite("SerialTest\n");
	}
}
