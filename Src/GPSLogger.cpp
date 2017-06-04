#include <Arduino_FreeRTOS.h>

#include "USBDevice.h"
#include "SerialUSB.h"

#include "Screens/ScreenManager.h"
#include "GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"

#include "Adafruit_SSD1306_STM32.h"

void TestThread(void *pvParameters)
{
	vTaskDelay(2000);

	SerialUSB.println("Inited");

	Adafruit_SSD1306 display(-1);
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
	display.setTextColor(WHITE);				// Assuming all subsequent commands draw in white color
	display.drawLine(0, 0, 128, 63, 1);
	display.display();

	while(1)
		vTaskDelay(2000);
}


int main(void)
{
	InitBoard();

	//SerialUSB.begin(115200);
	USBDeviceFS.reenumerate();
	USBDeviceFS.beginCDC();

	//initDisplay();
	initButtons();
	//initScreens();
	initGPS();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(TestThread, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
