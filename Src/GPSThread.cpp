#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPSThread.h"
#include "GPSDataModel.h"

// A GPS parser
NMEAGPS gpsParser;

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
//			Serial.write(c);
			gpsParser.handle(c);
		}
		
		if(gpsParser.available())
		{
			GPSDataModel::instance().processNewGPSFix(gpsParser.read());
			GPSDataModel::instance().processNewSatellitesData(gpsParser.satellites, gpsParser.sat_count);
		}			
			
		vTaskDelay(10);
	}
}
