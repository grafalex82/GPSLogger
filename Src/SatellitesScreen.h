#ifndef __SATELLITESSCREEN_H__
#define __SATELLITESSCREEN_H__

#include "Screen.h"

class SatellitesScreen : public Screen
{
public:
	SatellitesScreen();

	virtual void drawScreen() const;


private:
	SatellitesScreen( const SatellitesScreen &c );
	SatellitesScreen& operator=( const SatellitesScreen &c );

}; //SatellitesScreen

#endif //__SATELLITESSCREEN_H__
