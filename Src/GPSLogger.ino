#include <MapleFreeRTOS.h>
#include <Adafruit_SSD1306_STM32.h>
#include <TinyGPS++.h>


#include "ScreenManager.h"

//TinyGPSPlus gps;

void vLEDFlashTask(void *pvParameters) 
{
	for (;;) 
	{
		vTaskDelay(1000);
		digitalWrite(PC13, HIGH);
		vTaskDelay(150);
		digitalWrite(PC13, LOW);
		
		Serial.println((int)configMINIMAL_STACK_SIZE);
	}
}


// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(PC13, OUTPUT);
	
	Serial.begin(115200);

	initDisplay();
	initScreens();	
	//gpsSerial.begin(9600);
	
	
	xTaskCreate(vLEDFlashTask, (const signed char * const)"LED Task",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vUserInteractionTask, (const signed char * const)"UI Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	vTaskStartScheduler();
	
}

// the loop function runs over and over again forever
void loop()
{
	// Do not write any code here, it would not execute.

	

	/*
	if (Serial.available()) {
		gpsSerial.write(Serial.read());
	}*/

	// TODO: Process GPS data here

}

