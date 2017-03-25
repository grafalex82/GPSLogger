#ifndef __ODOMETERSCREEN_H__
#define __ODOMETERSCREEN_H__

#include "Screen.h"

class OdometerScreen : public Screen
{
	uint8 odometerIdx;
	bool inDetailsMode;
	
public:
	OdometerScreen(uint8 idx);

	virtual void drawScreen() const;
	
	virtual const char * getOkButtonText() const;
	
	virtual void onSelButton();
	virtual void onOkButton();

private:
	OdometerScreen( const OdometerScreen &c );
	OdometerScreen& operator=( const OdometerScreen &c );
	
	void drawBriefScreen() const;
	void drawDetailsScreen() const;
}; //OdometerScreen

#endif //__ODOMETERSCREEN_H__
