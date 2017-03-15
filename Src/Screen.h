#ifndef _SCREEN_H_
#define _SCREEN_H_

class Screen
{
	Screen * nextScreen;
	
public:
	Screen();
	//virtual ~Screen() {} //Virtual destructor is intentionally commented as it consumes a lot of RAM/FLASH but will never called

	virtual void drawScreen() const = 0;
	virtual void drawHeader() const;
	virtual void onSelButton();
	virtual void onOkButton();
	
	virtual const char * getSelButtonText() const;
	virtual const char * getOkButtonText() const;
	
	Screen * addScreen(Screen * screen);
};


#endif //_SCREEN_H_
