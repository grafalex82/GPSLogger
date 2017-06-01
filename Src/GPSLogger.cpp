
// FreeRTOS
#include <FreeRTOS.h>
#include "task.h"

#include "USBDevice.h"

//#include "Screens/ScreenManager.h"
#include "GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"


int main(void)
{
	InitBoard();

	//SerialUSB.begin(115200);

	//initDisplay();
	initButtons();
	//initScreens();
	initGPS();

	USBDeviceFS.reenumerate();
	USBDeviceFS.beginCDC();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
