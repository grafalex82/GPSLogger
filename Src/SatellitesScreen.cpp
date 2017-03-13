#include <Adafruit_SSD1306_STM32.h>
#include <MapleFreeRTOS821.h>
#include <NMEAGPS.h>

#include "SatellitesScreen.h"
#include "8x12Font.h"
#include "GPS.h"

extern Adafruit_SSD1306 display;
extern NMEAGPS::satellite_view_t satellites[ NMEAGPS_MAX_SATELLITES ];
extern uint8_t sat_count;

NMEAGPS::satellite_view_t l_satellites[ NMEAGPS_MAX_SATELLITES ];
uint8_t l_sat_count;

extern SemaphoreHandle_t xGPSDataMutex;

SatellitesScreen::SatellitesScreen()
{
}

void SatellitesScreen::drawScreen()
{
	xSemaphoreTake(xGPSDataMutex, portMAX_DELAY);
	memcpy(l_satellites, satellites, sizeof(l_satellites));
	l_sat_count = sat_count;
	xSemaphoreGive(xGPSDataMutex);
	
	display.setFont(NULL);
	display.setCursor(0,8);
	display.print(F("Sats: "));
	display.println(l_sat_count);
	
	uint8 width = 3;
	uint8 tracked = 0;
	for(uint8 sat = 0; sat < l_sat_count; sat++)
	{
		uint8 x = display.width() / 2 + sat * width;
		uint8 h = (float)l_satellites[sat].snr * 24 / 48;
		display.fillRect(x, display.height() - h, width - 1, h, 1);

		if (l_satellites[sat].tracked) // TODO: check if we are really interested in this
		{
			display.drawFastHLine(x, 8, width - 1, 1);
			tracked++;
		}
	}

	display.setCursor(0,16);
	display.print(F("Tracked: "));
	display.println(tracked);

}
