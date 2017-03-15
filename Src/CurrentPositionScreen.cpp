#include <Adafruit_SSD1306_STM32.h>
#undef degrees //defined in wirish.h
#include <NMEAGPS.h>

#include "8x12Font.h"
#include "Utils.h"
#include "ScreenManager.h"
#include "CurrentPositionScreen.h"

extern Adafruit_SSD1306 display;

extern gps_fix gpsData;

void CurrentPositionScreen::drawScreen() const
{
	static char latitudeString[17]; // 16 symbols + terminating zerno
	static char longtitudeString[17];

	// string template	
	strcpy(latitudeString, "LA: --\"--'--.-- ");
	strcpy(longtitudeString, "LO:---\"--'--.-- ");
	
	display.setFont(&Monospace8x12Font);
	display.setCursor(0,20);
	if(gpsData.valid.location)
	{
		// Latitude
		printNumber(latitudeString + 4, gpsData.latitudeDMS.degrees, 2);
		printNumber(latitudeString + 7, gpsData.latitudeDMS.minutes, 2);
		printNumber(latitudeString + 10, gpsData.latitudeDMS.seconds_whole, 2);
		printNumber(latitudeString + 13, gpsData.latitudeDMS.seconds_frac, 2);
		latitudeString[15] = gpsData.latitudeDMS.NS();
		
		// Longtitude
		printNumber(longtitudeString + 3, gpsData.longitudeDMS.degrees, 3);
		printNumber(longtitudeString + 7, gpsData.longitudeDMS.minutes, 2);
		printNumber(longtitudeString + 10, gpsData.longitudeDMS.seconds_whole, 2);
		printNumber(longtitudeString + 13, gpsData.longitudeDMS.seconds_frac, 2);
		longtitudeString[15] = gpsData.longitudeDMS.EW();
	}

	display.setCursor(0,20);
	display.print(latitudeString); // TODO GFX library wraps the last symbol to new line
	display.setCursor(0,32);
	display.print(longtitudeString);
}

const char * CurrentPositionScreen::getOkButtonText() const
{
	static const char text[] PROGMEM = "POI";
	return text;
}

void CurrentPositionScreen::onOkButton()
{
	static const char text[] PROGMEM = "Waypoint Saved!";
	showMessageBox(text);
}
