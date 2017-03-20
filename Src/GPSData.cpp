#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>

#include "FreeRTOSHelpers.h"
#include "GPSData.h"

GPSData gpsData;

GPSData::GPSData()
{
	// Initialize the mutex
	// TODO: consider migrating to FreeRTOS 9.x which allows allocating mutex statically
	xGPSDataMutex = xSemaphoreCreateMutex();
}

void GPSData::processNewGPSFix(const gps_fix & fix)
{
	MutexLocker lock(xGPSDataMutex);
	prev_fix = cur_fix;
	cur_fix = fix;
}

void GPSData::processNewSatellitesData(NMEAGPS::satellite_view_t * data, uint8_t count)
{
	MutexLocker lock(xGPSDataMutex);
	sattelitesData.parseSatellitesData(data, count);
}

gps_fix GPSData::getGPSFix() const
{
	MutexLocker lock(xGPSDataMutex);
	return cur_fix;	
}

GPSSatellitesData GPSData::getSattelitesData() const
{
	MutexLocker lock(xGPSDataMutex);
	return sattelitesData;
}
