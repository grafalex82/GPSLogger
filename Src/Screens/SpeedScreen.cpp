#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h>

#include "SpeedScreen.h"
#include "AltitudeScreen.h"
#include "TimeFont.h"
#include "8x12Font.h"
#include "PrintUtils.h"

#include "GPS/GPSDataModel.h"


extern Adafruit_SSD1306 display;

AltitudeScreen altitudeScreen;

SpeedScreen::SpeedScreen()
{
	addChildScreen(&altitudeScreen);
}

void SpeedScreen::drawScreen() const
{
	// Get the gps fix data
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();
	
	// Draw speed
	// TODO draw '----' if no GPS signal found. Requires new character in font
	char buf[7]; // 6 symbols + trailing zero
	strcpy(buf, "----");
	printNumber(buf, gpsFix.speed_kph(), 4, true); // TODO: Add leading spaces
	display.setFont(&TimeFont);
	display.setCursor(24,31);
	display.print(buf);
	
	// Draw speed units
	display.setFont(&Monospace8x12Font); // TODO: Use slightly larger bold font
	display.setCursor(90, 20);
	display.print("km/h");

	// Draw altitude
	strcpy(buf, "-----m");
	if(gpsFix.valid.altitude)
	{
		if(gpsFix.alt.whole >= 0)
			printNumber(buf, gpsFix.alt.whole, 5);
		else
			printNumber(buf+1, abs(gpsFix.alt.whole), 4); // there could be negative altitude
	}
	display.setFont(NULL);
	display.setCursor(90, 24);
	display.print(buf);

	// Draw heading letter
	display.setFont(&Monospace8x12Font); // TODO: Use slightly larger bold font
	display.setCursor(2, 20);
	if(gpsFix.valid.heading)
		display.print(headingAsLetter(gpsFix.hdg.whole));
	else
		display.print("--");

	// Draw heading value
	strcpy(buf, "---'");
	if(gpsFix.valid.heading)
		printNumber(buf, gpsFix.hdg.whole, 3);
	display.setFont(NULL);
	display.setCursor(0, 24);
	display.print(buf);
}

const char * SpeedScreen::headingAsLetter(uint16_t heading)
{
	if(heading < 22)
		return "N";
	if(heading < 67)
		return "NE";
	if(heading < 122)
		return "E";
	if(heading < 157)
		return "SE";
	if(heading < 202)
		return "S";
	if(heading < 247)
		return "SW";
	if(heading < 292)
		return "W";
	if(heading < 337)
		return "NW";
	
	return "N";
}

const char * SpeedScreen::getOkButtonText() const
{
	static const char text[] = "ALTITUDE";
	return text;
}
