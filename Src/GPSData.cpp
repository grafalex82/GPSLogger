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

gps_fix GPSData::getGPSFix()
{
	MutexLocker lock(xGPSDataMutex);
	return cur_fix;	
}
