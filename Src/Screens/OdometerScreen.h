#ifndef __ODOMETERSCREEN_H__
#define __ODOMETERSCREEN_H__

#include "Screen.h"
#include "OdometerActionScreen.h"

class OdometerScreen : public Screen
{
	uint8_t odometerIdx;
	bool inDetailsMode;
	OdometerActionScreen actionSelector;
	
public:
	OdometerScreen(uint8_t idx);

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
