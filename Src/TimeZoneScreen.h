#ifndef __TIMEZONESCREEN_H__
#define __TIMEZONESCREEN_H__

#include "Screen.h"

class TimeZoneScreen : public Screen
{
	int8_t hourDiff;
	int8_t minDiff;
	
public:
	TimeZoneScreen(int8_t hourDiff, int8_t minDiff);
	~TimeZoneScreen();

	virtual void drawScreen();
	virtual void onOkButton();
	
	virtual PROGMEM const char * getSelButtonText();
	virtual PROGMEM const char * getOkButtonText();

private:
	TimeZoneScreen( const TimeZoneScreen &c );
	TimeZoneScreen& operator=( const TimeZoneScreen &c );

}; //TimeZoneScreen

#endif //__TIMEZONESCREEN_H__
