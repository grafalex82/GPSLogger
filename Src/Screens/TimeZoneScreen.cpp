#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h>

#include "TimeZoneScreen.h"
#include "ScreenManager.h"
#include "TimeFont.h"
#include "PrintUtils.h"

#include "GPS/GPSDataModel.h"

extern Adafruit_SSD1306 display;

const int16_t timeZones[] = 
{
	0 * 60 + 00,	// +00:00
	1 * 60 + 00,	// +01:00
	2 * 60 + 00,	// +02:00
	3 * 60 + 00,	// +03:00
	3 * 60 + 30,	// +03:30
	4 * 60 + 00,	// +04:00
	4 * 60 + 30,	// +04:30
	5 * 60 + 00,	// +05:00
	5 * 60 + 30,	// +05:30
	5 * 60 + 45,	// +05:45
	6 * 60 + 00,	// +06:00
	6 * 60 + 30,	// +06:30
	7 * 60 + 00,	// +07:00
	8 * 60 + 00,	// +08:00
	9 * 60 + 00,	// +09:00
	9 * 60 + 30,	// +09:30
	10 * 60 + 00,	// +10:00
	11 * 60 + 00,	// +11:00
	12 * 60 + 00,	// +12:00
	13 * 60 + 00,	// +13:00
	
	-12 * 60 - 00,	// -12:00
	-11 * 60 - 00,	// -11:00
	-10 * 60 - 00,	// -10:00
	-9 * 60 - 00,	// -09:00
	-8 * 60 - 00,	// -08:00
	-7 * 60 - 00,	// -07:00
	-6 * 60 - 00,	// -06:00
	-5 * 60 - 00,	// -05:00
	-4 * 60 - 30,	// -04:30
	-4 * 60 - 00,	// -04:00
	-3 * 60 - 30,	// -03:30
	-3 * 60 - 00,	// -03:00
	-2 * 60 - 00,	// -02:00
	-1 * 60 - 00	// -01:00
};


TimeZoneScreen::TimeZoneScreen()
{
	timeZoneIdx = 2; // TODO: get the value in settings EEPROM
}

void TimeZoneScreen::drawScreen() const
{
	// Get the date/time adjusted by selected timezone value
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();
	int16_t timeZone = getCurrentTimeZone();
	NeoGPS::time_t dateTime = gpsFix.dateTime + timeZone * 60; //timeZone is in minutes
	
	// Draw the time string
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	printToDisplay("%02d:%02d", dateTime.hours, dateTime.minutes);
	
	// Draw the date and time zone strings
	display.setFont(NULL);
	display.setCursor(78,8);
	printToDisplay("%02d/%02d/%02d", dateTime.date, dateTime.month, dateTime.year);

	display.setCursor(78,16);
	display.print("  UTC");

	display.setCursor(78,24);
	display.print(timeZone < 0 ? '-' : '+'); // TODO implement char printing for printf
	printToDisplay("%02d:%02d", abs(timeZone) / 60, abs(timeZone) % 60);
}

void TimeZoneScreen::onSelButton()
{	
	timeZoneIdx++;
	timeZoneIdx %= sizeof(timeZones) / sizeof(timeZones[0]);
}

void TimeZoneScreen::onOkButton()
{
	//TODO: Apply time zone here and store it to Settings/EEPROM
	
	//TODO: consider handling a long press as a cancel operation
	
	backToParentScreen();
}

const char * TimeZoneScreen::getSelButtonText() const
{
	static const char text[] = "SELECT";
	return text;
}

const char * TimeZoneScreen::getOkButtonText() const
{
	static const char text[] = "OK";
	return text;
}

int16_t TimeZoneScreen::getCurrentTimeZone() const
{
	return timeZones[timeZoneIdx];
}
