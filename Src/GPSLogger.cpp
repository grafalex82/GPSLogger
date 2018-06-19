#include <Arduino_FreeRTOS.h>

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "TargetCPU/TargetCPU.h"
#include "LEDThread.h"
#include "ButtonsThread.h"
#include "SDThread.h"
#include "USBDebugLogger.h"
#include "SerialDebugLogger.h"
//#include "SdMscDriver.h"

int main(void)
{
	InitBoard();
	//initDebugSerial();

	portENABLE_INTERRUPTS(); // To allow halt() use HAL_Delay()

	halt(5);

	// Initialize SD card before initializing USB
	//if(!initSDIOThread())
	//	halt(7);

	//initUSB();

	//initDisplay();
	//initButtons();
	//initScreens();
	//initSDThread();
	//initGPS();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	//xTaskCreate(vSDThread, "SD Thread", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vDisplayTask, "Display Task", 768, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(xSDIOThread, "SD IO executor", 256, NULL, tskIDLE_PRIORITY + 3, NULL);
	//xTaskCreate(xSDTestThread, "SD test thread", 200, NULL, tskIDLE_PRIORITY + 3, NULL);
	//xTaskCreate(vGPSTask, "GPS Task", 256, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}

extern "C"
void vApplicationStackOverflowHook(xTaskHandle *pxTask,
								   signed char *pcTaskName)
{
	/* This function will get called if a task overflows its stack.
	 * If the parameters are corrupt then inspect pxCurrentTCB to find
	 * which was the offending task. */

	(void) pxTask;
	(void) pcTaskName;

	while (1)
		;
}

extern "C" void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.*/
	//taskDISABLE_INTERRUPTS();
	for( ;; );
}
