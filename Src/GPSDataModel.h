#ifndef __GPSDATA_H__
#define __GPSDATA_H__

#include "GPSSatellitesData.h"

// Forward declarations
class gps_fix;
class GPSSatellitesData;
class NMEAGPS;
struct NMEAGPS::satellite_view_t;
typedef void * QueueHandle_t;
typedef QueueHandle_t SemaphoreHandle_t;
class GPSOdometer;
class GPSOdometerData;

const uint8 ODOMERTERS_COUNT = 3;

/**
 * GPS data model. Encapsulates all the knowledge about various GPS related data in the device
 */
class GPSDataModel
{
public:
	GPSDataModel();
	
	void processNewGPSFix(const gps_fix & fix);
	void processNewSatellitesData(NMEAGPS::satellite_view_t * sattelites, uint8_t count);
	gps_fix getGPSFix() const;
	GPSSatellitesData getSattelitesData() const;
	
	float getVerticalSpeed() const;
	int timeDifference() const;
	
	// Odometers
	GPSOdometerData getOdometerData(uint8 idx) const;
	void resumeOdometer(uint8 idx);
	void pauseOdometer(uint8 idx);
	void resetOdometer(uint8 idx);
	void resumeAllOdometers();
	void pauseAllOdometers();
	void resetAllOdometers();
	
private:
	gps_fix cur_fix; /// most recent fix data
	gps_fix prev_fix; /// previously set fix data
	GPSSatellitesData sattelitesData; // Sattelites count and signal power
	GPSOdometer * odometers[ODOMERTERS_COUNT];
	bool odometerWasActive[ODOMERTERS_COUNT];
	
	SemaphoreHandle_t xGPSDataMutex;
	
	GPSDataModel( const GPSDataModel &c );
	GPSDataModel& operator=( const GPSDataModel &c );	
}; //GPSDataModel

/// A single instance of GPS data model
extern GPSDataModel gpsDataModel;

#endif //__GPSDATA_H__
