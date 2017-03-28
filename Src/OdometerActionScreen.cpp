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
	: SelectorScreen(actions, 4)
{
	odometerID = odometer;
}

void OdometerActionScreen::applySelection(uint8 idx)
{
	showMessageBox(actions[idx]);
}