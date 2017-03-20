#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"
#include "GPSData.h"
#include "Utils.h"

extern Adafruit_SSD1306 display;

TimeZoneScreen timeZoneScreen; //TODO Move it to CurrentTimeScreen class

void CurrentTimeScreen::drawScreen() const
{
	// Get the date/time adjusted by selected timezone value
	gps_fix gpsFix = gpsData.getGPSFix();
	NeoGPS::time_t dateTime = gpsFix.dateTime + timeZoneScreen.getCurrentTimeZone() * 60; //timeZone is in minutes

	// Fill the buffer with a message template	
	char buf[10];
	static const char * timeStringTemplate = "<00:00:00"; // '<' is remapeed to space
	strcpy(buf, timeStringTemplate);
	
	// Draw a '~' symbol if no time is available from GPS.
	if(!gpsFix.valid.time)
		buf[0] =  ';';  // ';' is remapped to '~'
	
	// Burn in current time
	printNumber(buf+1, dateTime.hours, 2);
	printNumber(buf+4, dateTime.minutes, 2);
	printNumber(buf+7, dateTime.seconds, 2);
	
	// TODO: display approximate time even if GPS is not available

	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);
}

CurrentTimeScreen::CurrentTimeScreen()
{
	addChildScreen(&timeZoneScreen);
}

