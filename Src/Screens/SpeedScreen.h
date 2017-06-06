#ifndef __SPEEDSCREEN_H__
#define __SPEEDSCREEN_H__

#include "ParentScreen.h"

class SpeedScreen : public ParentScreen
{
public:
	SpeedScreen();

	virtual void drawScreen() const;
	
	virtual const char * getOkButtonText() const;

	static const char * headingAsLetter(uint16_t heading);

private:
	SpeedScreen( const SpeedScreen &c );
	SpeedScreen& operator=( const SpeedScreen &c );
}; //SpeedScreen

#endif //__SPEEDSCREEN_H__
