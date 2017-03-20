#include <Adafruit_SSD1306_STM32.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>

#include "SatellitesScreen.h"
#include "8x12Font.h"
#include "GPSData.h"

// DebugScreen should not be here. I just needed to attach DebugScreen somewhere
#include "DebugScreen.h"
DebugScreen debugScreen;

extern Adafruit_SSD1306 display;
extern NMEAGPS::satellite_view_t satellites[ NMEAGPS_MAX_SATELLITES ];
extern uint8_t sat_count;

SatellitesScreen::SatellitesScreen()
{
	addChildScreen(&debugScreen);
}

void SatellitesScreen::drawScreen() const
{	
	// Try locking the semaphore as little as possible.
	// do not perform too much drawing while semaphore is locked, 
	// otherwise we will have to make another copy of data
	// xSemaphoreTake(xGPSDataMutex, portMAX_DELAY);
	// TODO: wrap satellites data into a data object so there is no need to lock semaphore
	
	// Draw a bar on each satellites in the list
	uint8 width = 3;
	uint8 tracked = 0;
	for(uint8 sat = 0; sat < sat_count; sat++)
	{
		uint8 x = display.width() / 2 + sat * width;
		uint8 snr = satellites[sat].snr;
		if (snr > 50) // Cap value with 50 dBm
			snr = 50;
		uint8 h = (float)snr * 22 / 50; // TODO: Remove hardcoded numbers. 22 is max height for a bar, 50 is max dBm
		display.fillRect(x, display.height() - h, width - 1, h, 1);

		// Draw a short stroke above the tracked satellite bar
		if (satellites[sat].tracked) // TODO: check if we are really interested in this
		{
			display.drawFastHLine(x, 9, width - 1, 1);
			tracked++;
		}
	}

	// other stuff
	uint8 sat_count_local = sat_count;
	gps_fix gpsFix = gpsData.getGPSFix(); 
	gps_fix::status_t status = gpsFix.status;

	// No more need to keep mutex
	//xSemaphoreGive(xGPSDataMutex);
	// TODO: wrap satellites data into a data object so there is no need to lock semaphore

	// Drawing stats
	display.setFont(NULL);
	display.setCursor(0,8);
	display.print("Sats: ");
	display.print(tracked);
	display.print("/");
	display.println(sat_count_local);
	if(gpsFix.valid.hdop)
		display.print(gpsFix.hdop);
	else
		display.print("-");
	display.print(" ");
	if(gpsFix.valid.vdop)
		display.println(gpsFix.vdop);
	else
	display.print("-");
	display.print("St: ");
	static const char * statuses[] = {"NONE", "EST", "TIME", "STD", "DGPS"};
	display.println(statuses[status]);
}
