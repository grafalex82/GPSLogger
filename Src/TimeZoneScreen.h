#ifndef __TIMEZONESCREEN_H__
#define __TIMEZONESCREEN_H__

#include "Screen.h"

class TimeZoneScreen : public Screen
{
public:
	TimeZoneScreen();

	virtual void drawScreen() const;
	virtual void onOkButton();
	
	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;

	//returns time zone as a number of minutes to add/substract from GPS time
	int16 getCurrentTimeZone() const {return timeZone;} 

private:
	TimeZoneScreen( const TimeZoneScreen &c );
	TimeZoneScreen& operator=( const TimeZoneScreen &c );

	// number of minutes to add/substract from GPS time
	int16 timeZone;
}; //TimeZoneScreen

#endif //__TIMEZONESCREEN_H__
