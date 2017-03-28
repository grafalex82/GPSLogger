#include "GPSOdometer.h"
#include "OdometerActionScreen.h"
#include "ScreenManager.h"

const char * actions[] = 
{
	"Start",
	"Resume",
	"Pause",
	"Reset"	
};

OdometerActionScreen::OdometerActionScreen(uint8 odometer)
{
	odometerID = odometer;
}

const char * OdometerActionScreen::getItemText(uint8 idx) const
{
	return actions[idx];	
}

uint8 OdometerActionScreen::getItemsCount() const
{
	return 4;	
}

void OdometerActionScreen::applySelection(uint8 idx)
{
	showMessageBox(actions[idx]);
}