#ifndef __SELECTORSCREEN_H__
#define __SELECTORSCREEN_H__

#include "Screen.h"

/// Selector Screen is a base class for screens that provides user with number of options to select.
/// The class implements iteration over items and displaying items text. Class does not handle items themselves.
/// That is because items may be generated in runtime by a class-ancestor
class SelectorScreen: public Screen
{
	uint8 currentIdx;

public:
	SelectorScreen();

	virtual void drawScreen() const;

	virtual const char * getOkButtonText() const;
	virtual const char * getSelButtonText() const;

	virtual void onSelButton();
	virtual void onOkButton();

protected:
	virtual const char * getItemText(uint8 idx) const = 0;
	virtual uint8 getItemsCount() const = 0;
	virtual void applySelection(uint8 idx) = 0;	

private:
	SelectorScreen( const SelectorScreen &c );
	SelectorScreen& operator=( const SelectorScreen &c );

	void drawCentered(const char * str, uint8 y) const;

}; //SelectorScreen

#endif //__SELECTORSCREEN_H__
