#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPS.h"

// A GPS parser
NMEAGPS gpsParser;
// GPS data
gps_fix gpsData;

void initGPS()
{
	// GPS is attached to Serial1
	Serial1.begin(9600);
}

void vGPSTask(void *pvParameters)
{
	for (;;)
	{
		while (gpsParser.available( Serial1 ))
			gpsData = gpsParser.read();
			
		vTaskDelay(10);
	}
}