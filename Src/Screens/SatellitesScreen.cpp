#include <Adafruit_SSD1306.h>
#include <Arduino_FreeRTOS.h>
#include <NMEAGPS.h>

#include "SatellitesScreen.h"
#include "8x12Font.h"
#include "PrintUtils.h"

#include "GPS/GPSDataModel.h"

// DebugScreen should not be here. I just needed to attach DebugScreen somewhere
#include "DebugScreen.h"
#include "MemDumpScreen.h"
DebugScreen debugScreen;
MemDumpScreen dumpScreen;

extern Adafruit_SSD1306 display;

SatellitesScreen::SatellitesScreen()
{
	addChildScreen(&debugScreen);
}

void SatellitesScreen::drawScreen() const
{	
	GPSSatellitesData satellites = GPSDataModel::instance().getSattelitesData();
	
	// Draw a bar on each satellites in the list
	uint8_t width = 3;
	uint8_t tracked = 0;
	uint8_t sat_count = satellites.getSattelitesCount();
	for(uint8_t sat = 0; sat < sat_count; sat++)
	{
		uint8_t x = display.width() / 2 + sat * width;
		uint8_t snr = satellites.getSatteliteSNR(sat);
		if (snr > 50) // Cap value with 50 dBm
			snr = 50;
		uint8_t h = (float)snr * 22 / 50; // TODO: Remove hardcoded numbers. 22 is max height for a bar, 50 is max dBm
		display.fillRect(x, display.height() - h, width - 1, h, 1);

		// Draw a short stroke above the tracked satellite bar
		if (satellites.isSatteliteTracked(sat))
		{
			display.drawFastHLine(x, 9, width - 1, 1);
			tracked++;
		}
	}

	// other stuff
	uint8_t sat_count_local = sat_count;
	gps_fix gpsFix = GPSDataModel::instance().getGPSFix();
	gps_fix::status_t status = gpsFix.status;

	// Drawing stats
	display.setFont(NULL);
	display.setCursor(0,8);
	display.print("Sats: ");
	display.print(tracked);
	display.print("/");
	display.println(sat_count_local);
	if(gpsFix.valid.hdop)
		display.print(FloatPrinter(gpsFix.hdop/1000, 4));
	else
		display.print("----");
	display.print(" ");
	if(gpsFix.valid.vdop)
		display.println(FloatPrinter(gpsFix.vdop/1000, 4));
	else
		display.println("----");
	display.print("St: ");
	static const char * statuses[] = {"NONE", "EST", "TIME", "STD", "DGPS"};
	display.println(statuses[status]);
}
