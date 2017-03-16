#ifndef __ALTITUDESCREEN_H__
#define __ALTITUDESCREEN_H__

#include "Screen.h"

class AltitudeScreen: public Screen
{
public:
	AltitudeScreen();

	virtual void drawScreen() const;

	virtual void onOkButton();

	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;

private:
	AltitudeScreen( const AltitudeScreen &c );
	AltitudeScreen& operator=( const AltitudeScreen &c );

}; //AltitudeScreen

#endif //__ALTITUDESCREEN_H__
