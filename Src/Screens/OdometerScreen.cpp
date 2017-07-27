#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h>

#include "GPS/GPSOdometer.h"
#include "GPS/GPSDataModel.h"

#include "OdometerScreen.h"
#include "SelectorScreen.h"
#include "ScreenManager.h"
#include "TimeFont.h"
#include "8x12Font.h"
#include "PrintUtils.h"

extern Adafruit_SSD1306 display;

OdometerScreen::OdometerScreen(uint8_t idx)
	: actionSelector(idx)
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
	GPSOdometerData odometer = GPSDataModel::instance().getOdometerData(odometerIdx);
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	
	if(odometer.isActive())
		display.print(FloatPrinter(odometer.getOdometerValue(), 4));
	else
		display.print("0000"); // TODO: print '----' when there is such symbol in the font
		
	// Draw distance units
	display.setFont(&Monospace8x12Font); // TODO: Use slightly larger bold font
	display.setCursor(64, 20);
	display.print("km");
		
	display.setFont(NULL);
	display.setCursor(64,22);
	display.print("dH: ");
	display.print(FloatPrinter(odometer.getAltitudeDifference(), 4));
	display.print("m");
}

void OdometerScreen::drawDetailsScreen() const
{
	GPSOdometerData odometer = GPSDataModel::instance().getOdometerData(odometerIdx);

	display.setFont(NULL);

	display.setCursor(0,8);
	display.print("Dist: ");
	display.print(FloatPrinter(odometer.getOdometerValue(), 4));
	
	display.setCursor(64,8);
	display.print("dH: ");
	display.print(FloatPrinter(odometer.getAltitudeDifference(), 4));

	display.setCursor(0,16);
	display.print("Vavg: ");
	display.print(FloatPrinter(odometer.getAvgSpeed(), 4));

	display.setCursor(64,16);
	display.print("Vmax: ");
	display.print(FloatPrinter(odometer.getMaxSpeed(), 4));

	display.setCursor(0,24);
	NeoGPS::time_t totalTime(odometer.getTotalTime());
	printToDisplay("T:%02d:%02d:%02d", totalTime.hours, totalTime.minutes, totalTime.seconds); // TODO: This may not fit if total time is more that 1 day

	display.setCursor(64,24);
	NeoGPS::time_t activeTime(odometer.getActiveTime());
	printToDisplay("A:%02d:%02d:%02d", activeTime.hours, activeTime.minutes, activeTime.seconds); // TODO: This may not fit if total time is more that 1 day
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
	
	if(!inDetailsMode)
		enterChildScreen(&actionSelector);
}
