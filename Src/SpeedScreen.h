#ifndef __SPEEDSCREEN_H__
#define __SPEEDSCREEN_H__

#include "Screen.h"

class SpeedScreen : public Screen
{
public:
	SpeedScreen();

	virtual void drawScreen() const;

	static const char * headingAsLetter(uint16 heading);

private:
	SpeedScreen( const SpeedScreen &c );
	SpeedScreen& operator=( const SpeedScreen &c );
}; //SpeedScreen

#endif //__SPEEDSCREEN_H__
