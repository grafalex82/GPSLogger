#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"

extern Adafruit_SSD1306 display;

TimeZoneScreen timeZoneScreen; //TODO Move it to CurrentTimeScreen class

extern gps_fix gpsData;

void CurrentTimeScreen::drawScreen()
{
	int h = gpsData.dateTime.hours;
	int m = gpsData.dateTime.minutes;
	int s = gpsData.dateTime.seconds;

	char buf[10];
	buf[0] = gpsData.valid.time ? '<' : ';';  // ';' is remapped to '~', '<' is remapped to space 
	buf[1] = 0x30 + h / 10;
	buf[2] = 0x30 + h % 10;
	buf[3] = ':';
	buf[4] = 0x30 + m / 10;
	buf[5] = 0x30 + m % 10;
	buf[6] = ':';
	buf[7] = 0x30 + s / 10;
	buf[8] = 0x30 + s % 10;
	buf[9] = 0;
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);
}

CurrentTimeScreen::CurrentTimeScreen()
{
	addChildScreen(&timeZoneScreen);
}

