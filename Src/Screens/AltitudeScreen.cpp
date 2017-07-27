#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h>

#include "AltitudeScreen.h"
#include "ScreenManager.h"
#include "TimeFont.h"
#include "8x12Font.h"
#include "GPS/GPSDataModel.h"
#include "PrintUtils.h"

extern Adafruit_SSD1306 display;

AltitudeScreen::AltitudeScreen()
{
}

void AltitudeScreen::drawScreen() const
{
	// Get the gps fix data
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();

	// Draw Altitude
	// TODO draw '----' if no GPS signal found. Requires new character in font
	char buf[7]; // 6 symbols + trailing zero
	strcpy(buf, "-----");
	if(gpsFix.valid.altitude)
	{
		if(gpsFix.alt.whole >= 0)
			printNumber(buf, gpsFix.alt.whole, 5); //TODO add leading spaces
		else
			printNumber(buf+1, abs(gpsFix.alt.whole), 4); // there could be negative altitude
	}
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);

	// Draw Altitude units
	display.setFont(&Monospace8x12Font); // TODO: Use slightly larger bold font
	display.setCursor(82, 20);
	display.print("m");

	// Draw vertical speed
	float vspeed = GPSDataModel::instance().getVerticalSpeed();
	if(!isnan(vspeed))
	{
		display.setFont(NULL);
		display.setCursor(80, 24);
		display.print(FloatPrinter(vspeed, 5));
		display.print("m/s"); //TODO: Add a space before 'm/s'. Requires text wrapping fix in GFX library
	}
}

void AltitudeScreen::onOkButton()
{
	backToParentScreen();
}

const char * AltitudeScreen::getSelButtonText() const
{
	static const char text[] = "";
	return text;
}

const char * AltitudeScreen::getOkButtonText() const
{
	static const char text[] = "BACK";
	return text;
}
