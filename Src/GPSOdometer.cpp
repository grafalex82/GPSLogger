#include <NMEAGPS.h>

#include "GPSOdometer.h"

bool isValid(const NeoGPS::Location_t & loc)
{
	return loc.lat() && loc.lon();
}

GPSOdometer::GPSOdometer()
{
	resetOdometer();
}

void GPSOdometer::processNewFix(const gps_fix & fix)
{
	if(active)
	{
		// Fill starting position if needed
		if(fix.valid.location && !isValid(startLocation))
			startLocation = fix.location;
			
		// Fill starting altitude if neede
		if(fix.valid.altitude && !startAltitude) // I know altitude can be zero, but real zero cm altutude would be very rare condition. Hope this is not a big deal
			startAltitude = fix.altitude_cm();
			
		// Fill starting times if needed
		if(fix.valid.time)
		{
			if(!startTime)
				startTime = fix.dateTime;
			if(!sessionStartTime)
				sessionStartTime = fix.dateTime;
		}
			
		// Increment the odometer
		if(fix.valid.location)
		{
			// but only if previous location is really valid
			if(isValid(lastLocation))
				odometer += NeoGPS::Location_t::DistanceKm(fix.location, lastLocation);
				
			// In any case store current (valid) fix
			lastLocation = fix.location;
		}
		
		// Store current altitude
		if(fix.valid.altitude)
			curAltitude = fix.altitude_cm();
			
		// update active time values
		if(fix.valid.time)
			activeTime = fix.dateTime - sessionStartTime;
		
		// update max speed value
		if(fix.valid.speed && fix.speed_kph() > maxSpeed)
			maxSpeed = fix.speed_kph();
	}	
	
	 //Total time can be updated regardless of active state
	 if(fix.valid.time && startTime)
		totalTime = fix.dateTime - startTime;
}

void GPSOdometer::startOdometer()
{
	active = true;
	
	// Reset session values
	sessionStartTime = 0;
	activeTime = 0;
}

void GPSOdometer::pauseOdometer()
{
	active = false;
	
	activeTimeAccumulator += activeTime;
	activeTime = 0;
}

void GPSOdometer::resetOdometer()
{
	active = false;

	startLocation.init();
	lastLocation.init();
	odometer = 0.;
	startAltitude = 0;
	curAltitude = 0;
	startTime = 0;
	sessionStartTime = 0;
	totalTime = 0;
	activeTime = 0;
	activeTimeAccumulator = 0;
	maxSpeed = 0.;
}

float GPSOdometer::getAvgSpeed() const 
{
	// Prevent division by zero
	if(activeTimeAccumulator == 0)
		return 0.;
		
	return odometer / getActiveTime();
}

float GPSOdometer::getDirectDistance() const
{
	return NeoGPS::Location_t::DistanceKm(lastLocation, startLocation);
	return 0;
}
