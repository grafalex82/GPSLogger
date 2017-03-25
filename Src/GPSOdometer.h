#ifndef __GPSODOMETER_H__
#define __GPSODOMETER_H__

#include <NMEAGPS.h>

class GPSOdometer
{
	bool active;
	
	NeoGPS::Location_t startLocation;
	NeoGPS::Location_t lastLocation;
	
	float odometer;
	int16 startAltitude;
	int16 curAltitude;
	
	clock_t startTime;				///! When odometer was turned on for the first time
	clock_t sessionStartTime;		///! When odometer was resumed for the current session
	clock_t totalTime;				///! Total time for the odometer (difference between now and startTime)
	clock_t activeTime;				///! Duration of the current session (difference between now and sessionStartTime)
	clock_t activeTimeAccumulator;	///! Sum of all active session duration (not including current one)
	
	float maxSpeed;
	
public:
	GPSOdometer();

	// odometer control
	void processNewFix(const gps_fix & fix);
	void startOdometer();
	void pauseOdometer();
	void resetOdometer();

	// getters
	bool isActive() const {return active;}
	float getOdometerValue() const {return odometer;}
	int16 getAltitudeDifference() const {return (curAltitude - startAltitude) / 100.;} // altitude is in cm
	clock_t getTotalTime() const {return totalTime;}
	clock_t getActiveTime() const {return activeTimeAccumulator + activeTime;}
	float getMaxSpeed() const {return maxSpeed;}
	float getAvgSpeed() const;
	float getDirectDistance() const;

}; //GPSOdometer

#endif //__GPSODOMETER_H__
