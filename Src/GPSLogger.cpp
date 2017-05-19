#include <MapleFreeRTOS821.h>

#include "Screens/ScreenManager.h"
#include "Buttons.h"
#include "GPSThread.h"

#include "Arduino.h"

extern "C" void _init(void)  {}
int __errno = 0;

void vLEDFlashTask(void *pvParameters) 
{
	for (;;) 
	{
		vTaskDelay(2000);
		digitalWrite(PC13, LOW);
		vTaskDelay(100);
		digitalWrite(PC13, HIGH);
	}
}


// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(PC13, OUTPUT);
	
	SerialUSB.begin(115200);

	initDisplay();
	initButtons();
	initScreens();
	initGPS();
	
	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vLEDFlashTask, "LED Task",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsTask, "Buttons Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();
}

// Arduino boilerplate function. Do not write any code here, it would not execute.
void loop()
{
}

