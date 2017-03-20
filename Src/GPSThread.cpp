#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPSThread.h"
#include "GPSData.h"

// A GPS parser
NMEAGPS gpsParser;

// Satellites in view
NMEAGPS::satellite_view_t satellites[ NMEAGPS_MAX_SATELLITES ];
// Number of satetllites in view
uint8_t sat_count;

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
			Serial.write(c);
			gpsParser.handle(c);
		}
		
		if(gpsParser.available())
		{
			gpsData.processNewGPSFix(gpsParser.read());

			memcpy(satellites, gpsParser.satellites, sizeof(satellites));
			sat_count = gpsParser.sat_count;
		}			
			
		vTaskDelay(10);
	}
}
