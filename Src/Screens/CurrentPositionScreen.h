#ifndef _POSITION_SCREEN_H_
#define _POSITION_SCREEN_H_

#include "Screen.h"

class CurrentPositionScreen: public Screen
{
public:
	virtual void drawScreen() const;

	virtual void onOkButton();

protected:
	const char * getOkButtonText() const;
};

#endif //_POSITION_SCREEN_H_
