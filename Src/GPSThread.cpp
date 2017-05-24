#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>
#include <SD.h>
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
	vTaskDelay(2000);
	// see if the card is present and can be initialized:
	Serial.print("Initializing SD card...");
	if (!SD.begin(PA4))
	{
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}
	Serial.println("card initialized.");

	File dataFile = SD.open("GPS.raw", FILE_WRITE);

	for (;;)
	{
		while(Serial1.available())
		{
			int c = Serial1.read();
			Serial.write(c);
			gpsParser.handle(c);

			dataFile.write(c);
		}
		
		if(gpsParser.available())
		{
			GPSDataModel::instance().processNewGPSFix(gpsParser.read());
			GPSDataModel::instance().processNewSatellitesData(gpsParser.satellites, gpsParser.sat_count);

			Serial.println("--- Flushing...");
			dataFile.flush();
		}			
			
		vTaskDelay(10);
	}
}
