#ifndef _POSITION_SCREEN_H_
#define _POSITION_SCREEN_H_

#include "Screen.h"

class CurrentPositionScreen: public Screen
{
public:
	virtual void drawScreen();

	virtual void onOkButton();

protected:
	const char * getOkButtonText();
};

#endif //_POSITION_SCREEN_H_
