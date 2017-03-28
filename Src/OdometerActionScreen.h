#ifndef __ODOMETERACTIONSCREEN_H__
#define __ODOMETERACTIONSCREEN_H__

#include "SelectorScreen.h"

class OdometerActionScreen: public SelectorScreen
{
	uint8 odometerID;
	
public:
	OdometerActionScreen(uint8 odometer);

protected:
	virtual const char * getItemText(uint8 idx) const;
	virtual uint8 getItemsCount() const;
	virtual void applySelection(uint8 idx);

private:
	OdometerActionScreen( const OdometerActionScreen &c );
	OdometerActionScreen& operator=( const OdometerActionScreen &c );
}; //OdometerActionScreen

#endif //__ODOMETERACTIONSCREEN_H__
