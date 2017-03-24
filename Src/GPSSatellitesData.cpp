#include <NMEAGPS.h>

#include "GPSSatellitesData.h"

GPSSatellitesData::GPSSatellitesData()
{
}

void GPSSatellitesData::parseSatellitesData(NMEAGPS::satellite_view_t * sattelites, uint8_t count)
{
	sat_count = count;
	for(uint8_t i = 0; i < count; i++)
	{
		satellitesData[i].snr = sattelites[i].snr;
		satellitesData[i].tracked = sattelites[i].tracked;
	}
}