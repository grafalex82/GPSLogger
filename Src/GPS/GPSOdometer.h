#ifndef __GPSODOMETER_H__
#define __GPSODOMETER_H__

#include <NMEAGPS.h>

// This class represents a single odometer data with no logic around
class GPSOdometerData
{
	// GPSOdometer and its data are basically a single object. The difference is only that data can be easily copied
	// while GPS odometer object is not supposed to. Additionally access to Odometer object is protected with a mutex 
	// in the model object
	// In order not to overcomplicte design I am allowing GPS Odometer to operate its data members directly.
	friend class GPSOdometer;
	
	bool active;
	
	NeoGPS::Location_t startLocation;
	NeoGPS::Location_t lastLocation;
	
	float odometer;
	int16_t startAltitude;
	int16_t curAltitude;
	
	clock_t startTime;				///! When odometer was turned on for the first time
	clock_t sessionStartTime;		///! When odometer was resumed for the current session
	clock_t totalTime;				///! Total time for the odometer (difference between now and startTime)
	clock_t activeTime;				///! Duration of the current session (difference between now and sessionStartTime)
	clock_t activeTimeAccumulator;	///! Sum of all active session duration (not including current one)
	
	float maxSpeed;

public:	
	GPSOdometerData();
	void reset();

	// getters
	bool isActive() const {return active;}
	float getOdometerValue() const {return odometer;}
	int16_t getAltitudeDifference() const {return (curAltitude - startAltitude) / 100.;} // altitude is in cm
	clock_t getTotalTime() const {return totalTime;}
	clock_t getActiveTime() const {return activeTimeAccumulator + activeTime;}
	float getMaxSpeed() const {return maxSpeed;}
	float getAvgSpeed() const;
	float getDirectDistance() const;
};

// This is an active odometer object that operates on its odometer data
class GPSOdometer
{
	GPSOdometerData data;

public:
	GPSOdometer();

	// odometer control
	void processNewFix(const gps_fix & fix);
	void startOdometer();
	void pauseOdometer();
	void resetOdometer();
	
	// Some data getters
	GPSOdometerData getData() {return data;}
	bool isActive() const {return data.isActive();}
}; //GPSOdometer

#endif //__GPSODOMETER_H__
