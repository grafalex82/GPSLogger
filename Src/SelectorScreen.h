#ifndef __SELECTORSCREEN_H__
#define __SELECTORSCREEN_H__

#include "Screen.h"

typedef void (*valueAcceptedFunc)();

struct SelectorRecord
{
	const char * valueStr;
	valueAcceptedFunc acceptFunc;
};

class SelectorScreen: public Screen
{
	SelectorRecord * records;
	uint8 count;
	uint8 currentIdx;

public:
	SelectorScreen(SelectorRecord * recs, uint8 cnt);

	virtual void drawScreen() const;

	virtual const char * getOkButtonText() const;
	virtual const char * getSelButtonText() const;

	virtual void onSelButton();
	virtual void onOkButton();
	

private:
	SelectorScreen( const SelectorScreen &c );
	SelectorScreen& operator=( const SelectorScreen &c );

	void drawCentered(const char * str, uint8 y) const;

}; //SelectorScreen

#endif //__SELECTORSCREEN_H__
