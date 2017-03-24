#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>

#include "FreeRTOSHelpers.h"
#include "GPSDataModel.h"

GPSDataModel gpsDataModel;

GPSDataModel::GPSDataModel()
{
	// Initialize the mutex
	// TODO: consider migrating to FreeRTOS 9.x which allows allocating mutex statically
	xGPSDataMutex = xSemaphoreCreateMutex();
}

void GPSDataModel::processNewGPSFix(const gps_fix & fix)
{
	MutexLocker lock(xGPSDataMutex);
	prev_fix = cur_fix;
	cur_fix = fix;
}

void GPSDataModel::processNewSatellitesData(NMEAGPS::satellite_view_t * sattelites, uint8_t count)
{
	MutexLocker lock(xGPSDataMutex);
	sattelitesData.parseSatellitesData(sattelites, count);
}

gps_fix GPSDataModel::getGPSFix() const
{
	MutexLocker lock(xGPSDataMutex);
	return cur_fix;	
}

GPSSatellitesData GPSDataModel::getSattelitesData() const
{
	MutexLocker lock(xGPSDataMutex);
	return sattelitesData;
}

float GPSDataModel::getVerticalSpeed() const
{
	MutexLocker lock(xGPSDataMutex);
	
	Serial.print("Curfix: ");
	Serial.println(cur_fix.valid.altitude);
	Serial.print("Prevfix: ");
	Serial.println(prev_fix.valid.altitude);
	
	// Return NAN to indicate vertical speed not available
	if(!cur_fix.valid.altitude || !prev_fix.valid.altitude)
	{
		Serial.println("V Speed is not available");
		return NAN;
	}
	
	return cur_fix.altitude() - prev_fix.altitude(); // Assuming that time difference between cur and prev fix is 1 second
}

int GPSDataModel::timeDifference() const
{
	MutexLocker lock(xGPSDataMutex);
	
	// Return NAN to indicate vertical speed not available
	if(!cur_fix.valid.altitude || !prev_fix.valid.altitude)
		return 0;
	
	return cur_fix.dateTime - prev_fix.dateTime;
}