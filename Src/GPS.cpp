#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <TinyGPS++.h>

#include "GPS.h"

// A GPS parsing library
TinyGPSPlus gps;

void initGPS()
{
	// GPS is attached to Serial1
	Serial1.begin(9600);
}

void vGPSTask(void *pvParameters)
{
	for (;;)
	{
		while(Serial1.available()) 
		{
			int c = Serial1.read();
			gps.encode(c);
			Serial.write(c);
			
			if (c == 0x0a)
			{
				Serial.println(gps.time.value());
			}
		}		
			
		vTaskDelay(2);
	}
}