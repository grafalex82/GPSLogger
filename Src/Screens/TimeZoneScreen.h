#ifndef TIMEZONESCREEN_H_
#define TIMEZONESCREEN_H_

#include "Screen.h"

#include <stdint.h>

class TimeZoneScreen : public Screen
{
public:
	TimeZoneScreen();

	virtual void drawScreen() const;
	virtual void onSelButton();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;

	//returns time zone as a number of minutes to add/substract from GPS time
	int16_t getCurrentTimeZone() const;

private:
	TimeZoneScreen( const TimeZoneScreen &c );
	TimeZoneScreen& operator=( const TimeZoneScreen &c );

	// An index in a list of time zones
	uint8_t timeZoneIdx;
}; //TimeZoneScreen

#endif //TIMEZONESCREEN_H_
