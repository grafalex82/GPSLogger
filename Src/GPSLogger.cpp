#include <Arduino_FreeRTOS.h>

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"
#include "SDThread.h"
#include "USBDebugLogger.h"

int main(void)
{
	InitBoard();

	portENABLE_INTERRUPTS(); // To allow halt() use HAL_Delay()

	//initUSB();
	//halt(5);

	//initDisplay();
	initButtons();
	//initScreens();
	//initSDThread();
	//initGPS();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	//xTaskCreate(vSDThread, "SD Thread", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vDisplayTask, "Display Task", 768, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", 256, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
