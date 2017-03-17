#ifndef __SATELLITESSCREEN_H__
#define __SATELLITESSCREEN_H__

#include "ParentScreen.h"

class SatellitesScreen : public ParentScreen  // TODO: This should inherit Screen, not ParentScreen. I just needed to attach DebugScreen somewhere
{
public:
	SatellitesScreen();

	virtual void drawScreen() const;

private:
	SatellitesScreen( const SatellitesScreen &c );
	SatellitesScreen& operator=( const SatellitesScreen &c );

}; //SatellitesScreen

#endif //__SATELLITESSCREEN_H__
