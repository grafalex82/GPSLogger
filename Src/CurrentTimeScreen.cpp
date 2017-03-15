#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"
#include "GPS.h"
#include "Utils.h"

extern Adafruit_SSD1306 display;

TimeZoneScreen timeZoneScreen; //TODO Move it to CurrentTimeScreen class

void CurrentTimeScreen::drawScreen()
{
	gps_fix gpsData = getGPSFixData();

	// Fill the buffer with a message template	
	char buf[10];
	static const char * timeStringTemplate = "<00:00:00"; // '<' is remapeed to space
	strcpy(buf, timeStringTemplate);
	
	if(!gpsData.valid.time)
		buf[0] =  ';';  // ';' is remapped to '~'
	
	printNumber(buf+1, gpsData.dateTime.hours, 2);
	printNumber(buf+4, gpsData.dateTime.minutes, 2);
	printNumber(buf+7, gpsData.dateTime.seconds, 2);
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);
}

CurrentTimeScreen::CurrentTimeScreen()
{
	addChildScreen(&timeZoneScreen);
}

