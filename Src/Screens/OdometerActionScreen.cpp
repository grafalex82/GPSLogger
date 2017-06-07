#include "GPS/GPSOdometer.h"
#include "GPS/GPSDataModel.h"
#include "OdometerActionScreen.h"
#include "ScreenManager.h"

OdometerActionScreen::OdometerActionScreen(uint8_t odometer)
{
	odometerID = odometer;
}

const char * OdometerActionScreen::getItemText(uint8_t idx) const
{
	if(idx == 1)
		return "Reset";

	// Assume idx=0 here, so actual text depends on odometer state	
	GPSOdometerData odometer = GPSDataModel::instance().getOdometerData(odometerID);
	
	// Active odometer can only be paused
	if(odometer.isActive())
		return "Pause";
		
	// Inactive is either already paused (and can be resumed) or even not started
	if(odometer.getTotalTime() != 0)
		return "Resume";
		
	return "Start";
}

uint8_t OdometerActionScreen::getItemsCount() const
{
	return 2;
}

void OdometerActionScreen::applySelection(uint8_t idx)
{
	GPSDataModel & gpsDataModel = GPSDataModel::instance();

	// Process reset action
	if(idx == 1)
	{
		showMessageBox("Reset");
		gpsDataModel.resetOdometer(odometerID);
		return;
	}

	// Active odometer can only be paused
	if(gpsDataModel.getOdometerData(odometerID).isActive())
	{
		showMessageBox("Pause");
		gpsDataModel.pauseOdometer(odometerID);
		return;
	}
	
	// Resuming/starting inactive odometer
	showMessageBox("Start/Resume");
	gpsDataModel.resumeOdometer(odometerID);
}
