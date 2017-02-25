#ifndef _SCREEN_H_
#define _SCREEN_H_

//TODO: move elsewhere
#define USE_PGM_STRING(x) reinterpret_cast<const __FlashStringHelper *>(x)

class Screen
{
	Screen * nextScreen;
	
public:
	Screen();
	virtual ~Screen() {}

	virtual void drawScreen() = 0;
	virtual void drawHeader();
	virtual void onSelButton();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText();
	virtual const char * getOkButtonText();
	
	Screen * addScreen(Screen * screen);
};


#endif //_SCREEN_H_
