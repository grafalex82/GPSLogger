#ifndef __GPSSATELLITESDATA_H__
#define __GPSSATELLITESDATA_H__

const uint8_t SAT_ARRAY_SIZE = 20; // Must match NMEAGPS_MAX_SATELLITES

// Forward declaration
class NMEAGPS;
//struct NMEAGPS::satellite_view_t;

class GPSSatellitesData
{
	// Partial copy of NMEAGPS::satellite_view_t trimmed to used data
	struct SatteliteData
	{
		uint8_t snr;
		bool tracked;
	};	
	
	SatteliteData satellitesData[SAT_ARRAY_SIZE];
	uint8_t sat_count;
	
public:
	GPSSatellitesData();

	void parseSatellitesData(NMEAGPS::satellite_view_t * sattelites, uint8_t count);
	
	uint8_t getSattelitesCount() const {return sat_count;}
	uint8_t getSatteliteSNR(uint8_t sat) const {return satellitesData[sat].snr;}
	bool isSatteliteTracked(uint8_t sat) const {return satellitesData[sat].tracked;}
};

#endif //__GPSSATELLITESDATA_H__
