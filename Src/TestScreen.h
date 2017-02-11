
#ifndef __TESTSCREEN_H__
#define __TESTSCREEN_H__

#include "ParentScreen.h"

class TestScreen : public ParentScreen
{
	const char * str;

public:
	TestScreen(const char * str);
	~TestScreen();

	virtual void drawScreen();

private:
	TestScreen( const TestScreen &c );
	TestScreen& operator=( const TestScreen &c );

}; //TestScreen

#endif //__TESTSCREEN_H__
