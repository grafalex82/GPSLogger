#ifndef __PARENTSCREEN_H__
#define __PARENTSCREEN_H__

#include <avr/pgmspace.h>

#include "Screen.h"

class ParentScreen : public Screen
{
	Screen * childScreen;
	
public:
	ParentScreen();

	Screen * addChildScreen(Screen * screen);

	virtual void onOkButton();

private:
	ParentScreen( const ParentScreen &c );
	ParentScreen& operator=( const ParentScreen &c );
};

#endif //__PARENTSCREEN_H__
