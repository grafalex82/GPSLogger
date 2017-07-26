#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"
#include "GPS/GPSDataModel.h"
#include "PrintUtils.h"

extern Adafruit_SSD1306 display;

TimeZoneScreen timeZoneScreen; //TODO Move it to CurrentTimeScreen class

void CurrentTimeScreen::drawScreen() const
{
	// Get the date/time adjusted by selected timezone value
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();
	NeoGPS::time_t dateTime = gpsFix.dateTime + timeZoneScreen.getCurrentTimeZone() * 60; //timeZone is in minutes

	// TODO: display approximate time even if GPS is not available

	display.setFont(&TimeFont);
	display.setCursor(0,31);

	// Draw a '~' symbol if no time is available from GPS.
	display.print(gpsFix.valid.time ? '<' : ';'); // '<' is remapeed to space, ';' is remapped to '~'
	printToDisplay("%02d:%02d:%02d", dateTime.hours, dateTime.minutes, dateTime.seconds);
}

CurrentTimeScreen::CurrentTimeScreen()
{
	addChildScreen(&timeZoneScreen);
}

