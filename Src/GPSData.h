#ifndef __GPSDATA_H__
#define __GPSDATA_H__

#include "GPSSatellitesData.h"

// Forward declarations
class gps_fix;
class GPSSatellitesData;
typedef void * QueueHandle_t;
typedef QueueHandle_t SemaphoreHandle_t;

/**
 * GPS data model. Encapsulates all the knowledge about various GPS related data in the device
 */
class GPSData
{
public:
	GPSData();
	
	void processNewGPSFix(const gps_fix & fix);
	void processNewSatellitesData(NMEAGPS::satellite_view_t * data, uint8_t count);
	gps_fix getGPSFix() const;
	GPSSatellitesData getSattelitesData() const;
	
private:
	gps_fix cur_fix; /// most recent fix data
	gps_fix prev_fix; /// previously set fix data
	GPSSatellitesData sattelitesData;
	
	SemaphoreHandle_t xGPSDataMutex;
	
	GPSData( const GPSData &c );
	GPSData& operator=( const GPSData &c );	
}; //GPSData

/// A single instance of GPS data
extern GPSData gpsData;

#endif //__GPSDATA_H__
