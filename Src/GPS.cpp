#include <HardwareSerial.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPS.h"

// A GPS parser
NMEAGPS gpsParser;
// GPS data
gps_fix gpsData;

// Satellites in view
NMEAGPS::satellite_view_t satellites[ NMEAGPS_MAX_SATELLITES ];
// Number of satetlittes in view
uint8_t sat_count;

// TODO: Migrate to FreeRTOS 9.0 which allows allocating mutex statically
SemaphoreHandle_t xGPSDataMutex = NULL;

void initGPS()
{
	// GPS is attached to Serial1
	Serial1.begin(9600);
	
	// Initialize the mutex
	xGPSDataMutex = xSemaphoreCreateMutex();
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
			xSemaphoreTake(xGPSDataMutex, portMAX_DELAY);
			gpsData = gpsParser.read();
			memcpy(satellites, gpsParser.satellites, sizeof(satellites));
			sat_count = gpsParser.sat_count;
			xSemaphoreGive(xGPSDataMutex);
		}
			
			
		vTaskDelay(10);
	}
}

gps_fix getGPSFixData()
{
	gps_fix ret;
	
	xSemaphoreTake(xGPSDataMutex, portMAX_DELAY); // TODO consider using RAII mutex locker
	ret = gpsData;
	xSemaphoreGive(xGPSDataMutex);
	
	return ret;
}
