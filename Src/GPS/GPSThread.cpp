#include <SerialUART.h>
#include <SerialUSB.h>
#include <Arduino_FreeRTOS.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPSThread.h"
#include "GPSDataModel.h"

#include "SDThread.h"

// A GPS parser
NMEAGPS gpsParser;

void initGPS()
{
	// GPS is attached to Serial1
	SerialUART1.begin(9600);
}

void vGPSTask(void *pvParameters)
{
	uint8_t maxLen = 0;

	for (;;)
	{
		//Receive one line from GPS
		char * buf = requestRawGPSBuffer();
		uint8_t len = 0;

		while(true)
		{
			// Wait for a symbol
			while(!SerialUART1.available())
				vTaskDelay(3);

			int c = SerialUART1.read();

			// Handle received byte
			gpsParser.handle(c);
			//SerialUSB.write(c);
			buf[len++] = c;

			// Reached end of line
			if(c == '\n')
				break;

			// Buffer overrun protection
			if(len == maxRawGPSDataLen)
			{
				buf[len] = '\n';
				break;
			}
		}
		
		if(len > maxLen)
		{
			maxLen = len;
			SerialUSB.print("=== New max len detected: ");
			SerialUSB.println(maxLen);
		}

		//Send received raw data to SD thread
		ackRawGPSData(len);

		// Update GPS model data
		if(gpsParser.available())
		{
			GPSDataModel::instance().processNewGPSFix(gpsParser.read());
			GPSDataModel::instance().processNewSatellitesData(gpsParser.satellites, gpsParser.sat_count);
		}

		vTaskDelay(10);
	}
}
