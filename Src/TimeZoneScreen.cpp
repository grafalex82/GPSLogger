#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "TimeZoneScreen.h"
#include "ScreenManager.h"
#include "TimeFont.h"
#include "GPS.h"
#include "Utils.h"

extern Adafruit_SSD1306 display;

TimeZoneScreen::TimeZoneScreen()
{
	timeZone = -150; // TODO: get the value in settings EEPROM
}

void TimeZoneScreen::drawScreen() const
{
	// Get the date/time adjusted by selected timezone value
	gps_fix gpsData = getGPSFixData();
	NeoGPS::time_t dateTime = gpsData.dateTime + timeZone * 60; //timeZone is in minutes

	// Prepare current time string
	static const char * timeTemplate = "00:00";
	char timeBuf[6];
	strcpy(timeBuf, timeTemplate);
	printNumber(timeBuf, dateTime.hours, 2);
	printNumber(timeBuf+3, dateTime.minutes, 2);
	
	// Prepare date string
	static const char * dateTemplate = "00/00/00";
	char dateBuf[9];
	strcpy(dateBuf, dateTemplate);
	printNumber(dateBuf, dateTime.date, 2);
	printNumber(dateBuf+3, dateTime.month, 2);
	printNumber(dateBuf+6, dateTime.year, 2);

	// Prepare time zone string
	static const char * timeZoneTemplate = "+00:00";
	char timeZoneBuf[7];
	strcpy(timeZoneBuf, timeZoneTemplate);
	timeZoneBuf[0] = timeZone < 0 ? '-' : '+';
	printNumber(timeZoneBuf+1, abs(timeZone) / 60, 2);
	printNumber(timeZoneBuf+4, abs(timeZone) % 60, 2);
	
	// Draw the time string
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(timeBuf);
	
	// Draw the date and time zone strings
	display.setFont(NULL);
	display.setCursor(78,8);
	display.print(dateBuf);
	display.setCursor(78,16);
	display.print("  UTC");
	display.setCursor(78,24);
	display.print(timeZoneBuf);
}

void TimeZoneScreen::onOkButton()
{
	//TODO: Apply time zone here
	
	backToParentScreen();
}

const char * TimeZoneScreen::getSelButtonText() const
{
	static const char text[] PROGMEM = "SELECT";
	return text;
}

const char * TimeZoneScreen::getOkButtonText() const
{
	static const char text[] PROGMEM = "OK";
	return text;
}
