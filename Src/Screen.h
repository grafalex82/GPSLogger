#ifndef _SCREEN_H_
#define _SCREEN_H_

class Screen
{
	Screen * nextScreen;
	
public:
	Screen();
	//virtual ~Screen() {} //Virtual destructor is intentionally commented as it consumes a lot of RAM/FLASH but will never called

	virtual void drawScreen() = 0;
	virtual void drawHeader();
	virtual void onSelButton();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText();
	virtual const char * getOkButtonText();
	
	Screen * addScreen(Screen * screen);
};


#endif //_SCREEN_H_
