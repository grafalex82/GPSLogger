#include "NMEAGPS.h"

#include "GPSOdometer.h"

bool isValid(const NeoGPS::Location_t & loc)
{
	return loc.lat() && loc.lon();
}

GPSOdometerData::GPSOdometerData()
{
	reset();
}

void GPSOdometerData::reset()
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

float GPSOdometerData::getAvgSpeed() const
{
	// Prevent division by zero
	if(activeTimeAccumulator == 0)
		return 0.;
	
	return odometer / getActiveTime();
}

float GPSOdometerData::getDirectDistance() const
{
	return NeoGPS::Location_t::DistanceKm(lastLocation, startLocation);
}

GPSOdometer::GPSOdometer()
{
	data.reset();
}

void GPSOdometer::processNewFix(const gps_fix & fix)
{
	if(data.active)
	{
		// Fill starting position if needed
		if(fix.valid.location && !isValid(data.startLocation))
			data.startLocation = fix.location;
			
		// Fill starting altitude if neede
		if(fix.valid.altitude && !data.startAltitude) // I know altitude can be zero, but real zero cm altutude would be very rare condition. Hope this is not a big deal
			data.startAltitude = fix.altitude_cm();
			
		// Fill starting times if needed
		if(fix.valid.time)
		{
			if(!data.startTime)
				data.startTime = fix.dateTime;
			if(!data.sessionStartTime)
				data.sessionStartTime = fix.dateTime;
		}
			
		// Increment the odometer
		if(fix.valid.location)
		{
			// but only if previous location is really valid
			if(isValid(data.lastLocation))
				data.odometer += NeoGPS::Location_t::DistanceKm(fix.location, data.lastLocation);
				
			// In any case store current (valid) fix
			data.lastLocation = fix.location;
		}
		
		// Store current altitude
		if(fix.valid.altitude)
			data.curAltitude = fix.altitude_cm();
			
		// update active time values
		if(fix.valid.time)
			data.activeTime = fix.dateTime - data.sessionStartTime;
		
		// update max speed value
		if(fix.valid.speed && fix.speed_kph() > data.maxSpeed)
			data.maxSpeed = fix.speed_kph();
	}	
	
	 //Total time can be updated regardless of active state
	 if(fix.valid.time && data.startTime)
		data.totalTime = fix.dateTime - data.startTime;
}

void GPSOdometer::startOdometer()
{
	data.active = true;
	
	// Reset session values
	data.sessionStartTime = 0;
	data.activeTime = 0;
}

void GPSOdometer::pauseOdometer()
{
	data.active = false;
	
	data.activeTimeAccumulator += data.activeTime;
	data.activeTime = 0;
}

void GPSOdometer::resetOdometer()
{
	data.reset();
}
