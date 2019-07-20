#include <Arduino_FreeRTOS.h>

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"
#include "SDThread.h"
#include "USBDebugLogger.h"
//#include "SerialDebugLogger.h"
//#include "SdMscDriver.h"
#include "Thread.h"

static Thread<configMINIMAL_STACK_SIZE> ledThread(vLEDThread, "LED Thread", nullptr, tskIDLE_PRIORITY + 1);
static Thread<768>						displayThread(vDisplayTask, "Display Task", nullptr, tskIDLE_PRIORITY + 2);
static Thread<configMINIMAL_STACK_SIZE> buttonsThread(vButtonsThread, "Buttons Thread", nullptr, tskIDLE_PRIORITY + 2);
//static Thread<256>						gpsThread(vGPSTask, "GPS Thread", nullptr, tskIDLE_PRIORITY + 3);
static Thread<768>						sdThread(vSDThread, "SD Thread", nullptr, tskIDLE_PRIORITY + 1);

//xTaskCreate(xSDIOThread, "SD IO executor", 256, NULL, tskIDLE_PRIORITY + 3, NULL);
//xTaskCreate(xSDTestThread, "SD test thread", 200, NULL, tskIDLE_PRIORITY + 3, NULL);


int main(void)
{
	InitBoard();
	//initDebugSerial();

	portENABLE_INTERRUPTS(); // To allow halt() use HAL_Delay()

	// Initialize SD card before initializing USB
//	if(!initSDIOThread())
//		halt(7);

	initUSB();

	initButtons();
	//initSDThread();

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
