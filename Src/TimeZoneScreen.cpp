#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

#include "TimeZoneScreen.h"
#include "ScreenManager.h"
#include "TimeFont.h"

extern Adafruit_SSD1306 display;


TimeZoneScreen::TimeZoneScreen(int8_t hourDiff, int8_t minDiff )
	: hourDiff(hourDiff)
	, minDiff(minDiff)
{
}

TimeZoneScreen::~TimeZoneScreen()
{
}


void TimeZoneScreen::drawScreen()
{
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print("12:34");
	
	display.setFont(NULL);
	display.setCursor(78,8);
	display.print(F("04/02/17")); //TODO Print correct date
	display.setCursor(78,16);
	display.print(F(" UTC"));

	char buf[7];
	buf[0] = '+';
	if (hourDiff < 0)
	{
		buf[0] = '-';
		hourDiff = 0 - hourDiff;
	}
	
	buf[1] = hourDiff / 10 + '0';
	buf[2] = hourDiff % 10 + '0';
	buf[3] = ':';
	buf[4] = minDiff / 10 + '0';
	buf[5] = minDiff % 10 + '0';
	buf[6] = 0;
	display.setCursor(78,24);
	display.print(buf);
}

void TimeZoneScreen::onOkButton()
{
	//TODO: Apply time zone here
	
	backToParentScreen();
}

const char * TimeZoneScreen::getSelButtonText()
{
	static const char text[] PROGMEM = "SELECT";
	return text;
}

const char * TimeZoneScreen::getOkButtonText()
{
	static const char text[] PROGMEM = "OK";
	return text;
}

