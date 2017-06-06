#ifndef __ODOMETERACTIONSCREEN_H__
#define __ODOMETERACTIONSCREEN_H__

#include "SelectorScreen.h"

class OdometerActionScreen: public SelectorScreen
{
	uint8_t odometerID;
	
public:
	OdometerActionScreen(uint8_t odometer);

protected:
	virtual const char * getItemText(uint8_t idx) const;
	virtual uint8_t getItemsCount() const;
	virtual void applySelection(uint8_t idx);

private:
	OdometerActionScreen( const OdometerActionScreen &c );
	OdometerActionScreen& operator=( const OdometerActionScreen &c );
}; //OdometerActionScreen

#endif //__ODOMETERACTIONSCREEN_H__
