#include <NMEAGPS.h>

#include "GPSSatellitesData.h"

GPSSatellitesData::GPSSatellitesData()
{
}

void GPSSatellitesData::parseSatellitesData(NMEAGPS::satellite_view_t * data, uint8_t count)
{
	sat_count = count;
	for(uint8_t i = 0; i < count; i++)
	{
		satellitesData[i].snr = data->snr;
		satellitesData[i].tracked = data->tracked;
	}
}