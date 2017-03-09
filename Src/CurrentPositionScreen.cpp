#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "8x12Font.h"

#include "ScreenManager.h"
#include "CurrentPositionScreen.h"

extern Adafruit_SSD1306 display;

extern gps_fix gpsData;

void CurrentPositionScreen::drawScreen()
{
	display.setFont(&Monospace8x12Font);
	display.setCursor(0,20);
	if(gpsData.valid.location)
	{
		display.setCursor(0,20);
		display.print(gpsData.latitudeL());
		display.setCursor(0,32);
		display.print(gpsData.longitudeL());
	}
	else
	{
		display.setCursor(0,20);
		display.print("LA: --\"--'--.--");
		display.setCursor(0,32);
		display.print("LO:---\"--'--.--");
	}
}

const char * CurrentPositionScreen::getOkButtonText()
{
	static const char text[] PROGMEM = "POI";
	return text;
}

void CurrentPositionScreen::onOkButton()
{
	static const char text[] PROGMEM = "Waypoint Saved!";
	showMessageBox(text);
}
