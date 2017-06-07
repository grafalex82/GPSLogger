#ifndef __DEBUGSCREEN_H__
#define __DEBUGSCREEN_H__

#include "Screen.h"

class DebugScreen : public Screen
{
public:
	DebugScreen();

	virtual void drawScreen() const;
	virtual void onSelButton();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;

private:
	DebugScreen( const DebugScreen &c );
	DebugScreen& operator=( const DebugScreen &c );
	
	uint8_t currentPage;
}; //DebugScreen

#endif //__DEBUGSCREEN_H__
