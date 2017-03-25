#include <Adafruit_SSD1306_STM32.h>
#include <NMEAGPS.h>

#include "OdometerScreen.h"
#include "GPSOdometer.h"
#include "TimeFont.h"
#include "8x12Font.h"
#include "GPSDataModel.h"
#include "Utils.h"

extern Adafruit_SSD1306 display;

OdometerScreen::OdometerScreen(uint8 idx)
{
	odometerIdx = idx;
	inDetailsMode = false;
}

void OdometerScreen::drawScreen() const
{
	if(inDetailsMode)
		drawDetailsScreen();
	else
		drawBriefScreen();
}

void OdometerScreen::drawBriefScreen() const
{
	GPSOdometer odometer = gpsDataModel.getOdometer(odometerIdx);
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	
	if(odometer.isActive())
		display.print(odometer.getOdometerValue()); // TODO: Use exactly 4 symbols. Consider adding a helper function
	else
		display.print("0000"); // TODO: print '----' when there is such symbol in the font
		
	// Draw distance units
	display.setFont(&Monospace8x12Font); // TODO: Use slightly larger bold font
	display.setCursor(64, 20);
	display.print("km");
		
	display.setFont(NULL);
	display.setCursor(64,22);
	display.print("dH: ");
	display.print(odometer.getAltitudeDifference()); // TODO: Use exactly 4 symbols. Consider adding a helper function
	display.print("m");
}

void OdometerScreen::drawDetailsScreen() const
{
	GPSOdometer odometer = gpsDataModel.getOdometer(odometerIdx);

	display.setFont(NULL);

	display.setCursor(0,8);
	display.print("Dist: ");
	display.print(odometer.getOdometerValue()); // TODO: Use exactly 4 symbols. Consider adding a helper function
	
	display.setCursor(64,8);
	display.print("dH: ");
	display.print(odometer.getAltitudeDifference()); // TODO: Use exactly 4 symbols. Consider adding a helper function

	display.setCursor(0,16);
	display.print("Vavg: ");
	display.print(odometer.getAvgSpeed()); // TODO: Use exactly 4 symbols. Consider adding a helper function

	display.setCursor(64,16);
	display.print("Vmax: ");
	display.print(odometer.getMaxSpeed()); // TODO: Use exactly 4 symbols. Consider adding a helper function

	display.setCursor(0,24);
	display.print("T:12:34:56");

	display.setCursor(64,24);
	display.print("A:21:43:65");
}

const char * OdometerScreen::getOkButtonText() const
{
	static const char details[] = "DETAILS";
	static const char back[] = "BACK";

	if(inDetailsMode)
		return back;
		
	return details;
}

void OdometerScreen::onSelButton()
{
	// We will be in brief mode when next time get back to this screen
	inDetailsMode = false;
	
	// Do usual stuff
	Screen::onSelButton();
}


void OdometerScreen::onOkButton()
{
	// just toggle mode
	inDetailsMode = !inDetailsMode;
}
