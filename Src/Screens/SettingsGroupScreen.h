#ifndef __SettingsGroupScreen_H__
#define __SettingsGroupScreen_H__

#include "Screen.h"

class SettingsGroupScreen: public Screen
{
public:
	SettingsGroupScreen();

	virtual void drawScreen() const;


private:
	SettingsGroupScreen( const SettingsGroupScreen &c );
	SettingsGroupScreen& operator=( const SettingsGroupScreen &c );

	void drawCentered(const char * str, uint8_t y) const;

}; //SettingsGroupScreen

#endif //__SettingsGroupScreen_H__
