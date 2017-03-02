#ifndef __TIMEZONESCREEN_H__
#define __TIMEZONESCREEN_H__

#include "Screen.h"

class TimeZoneScreen : public Screen
{
public:
	TimeZoneScreen();
	~TimeZoneScreen();

	virtual void drawScreen();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText();
	virtual const char * getOkButtonText();

private:
	TimeZoneScreen( const TimeZoneScreen &c );
	TimeZoneScreen& operator=( const TimeZoneScreen &c );

	// TODO Change this to a static list of timezones
	static int8_t hourDiff;
	static int8_t minDiff;
}; //TimeZoneScreen

#endif //__TIMEZONESCREEN_H__
