#ifndef MEMDUMPSCREEN_H
#define MEMDUMPSCREEN_H

#include "Screen.h"

class MemDumpScreen: public Screen
{
public:
	MemDumpScreen();

	virtual void drawScreen() const;
	virtual void onSelButton();
	virtual void onOkButton();

	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;

private:
	MemDumpScreen( const MemDumpScreen &c );
	MemDumpScreen& operator=( const MemDumpScreen &c );

	uint8_t * displayAddr;
};

#endif // MEMDUMPSCREEN_H
