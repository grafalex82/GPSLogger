#include <Adafruit_SSD1306.h>
#undef degrees //defined in wirish.h
#include <NMEAGPS.h>

#include "8x12Font.h"
#include "PrintUtils.h"
#include "ScreenManager.h"
#include "CurrentPositionScreen.h"
#include "GPS/GPSDataModel.h"

extern Adafruit_SSD1306 display;

void CurrentPositionScreen::drawScreen() const
{
	// Get the current Fix data
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();
	
	static char latitudeString[17]; // 16 symbols + terminating zerno
	static char longtitudeString[17];

	// string template	
	strcpy(latitudeString, "LA: --\"--'--.-- ");
	strcpy(longtitudeString, "LO:---\"--'--.-- ");
	
	display.setFont(&Monospace8x12Font);
	display.setCursor(0,20);
	if(gpsFix.valid.location)
	{
		// Latitude
		printNumber(latitudeString + 4, gpsFix.latitudeDMS.degrees, 2);
		printNumber(latitudeString + 7, gpsFix.latitudeDMS.minutes, 2);
		printNumber(latitudeString + 10, gpsFix.latitudeDMS.seconds_whole, 2);
		printNumber(latitudeString + 13, gpsFix.latitudeDMS.seconds_frac, 2);
		latitudeString[15] = gpsFix.latitudeDMS.NS();
		
		// Longtitude
		printNumber(longtitudeString + 3, gpsFix.longitudeDMS.degrees, 3);
		printNumber(longtitudeString + 7, gpsFix.longitudeDMS.minutes, 2);
		printNumber(longtitudeString + 10, gpsFix.longitudeDMS.seconds_whole, 2);
		printNumber(longtitudeString + 13, gpsFix.longitudeDMS.seconds_frac, 2);
		longtitudeString[15] = gpsFix.longitudeDMS.EW();
	}

	display.setCursor(0,20);
	display.print(latitudeString); // TODO GFX library wraps the last symbol to new line
	display.setCursor(0,32);
	display.print(longtitudeString);
}

const char * CurrentPositionScreen::getOkButtonText() const
{
	static const char text[] = "POI";
	return text;
}

void CurrentPositionScreen::onOkButton()
{
	static const char text[] = "Waypoint Saved!";
	showMessageBox(text);
}
