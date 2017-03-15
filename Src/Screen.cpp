#include <avr/pgmspace.h>

#include <Adafruit_SSD1306_STM32.h>

#include "Screen.h"
#include "ScreenManager.h"

extern Adafruit_SSD1306 display;

Screen::Screen()
{
	nextScreen = this;
}

void Screen::drawHeader() const
{
	display.setFont(NULL);
	display.setCursor(20, 0);
	display.print('\x1e');
	display.print(getSelButtonText());
	
	display.setCursor(80, 0);
	display.print('\x1e');
	display.print(getOkButtonText());
}

void Screen::onSelButton()
{
	setCurrentScreen(nextScreen);
}

void Screen::onOkButton()
{
	//nothing by default. implemented in derived classes
}

Screen * Screen::addScreen(Screen * screen)
{
	screen->nextScreen = nextScreen;
	nextScreen = screen;
	return screen;
}

const char * Screen::getSelButtonText() const
{
	static const char text[] PROGMEM = "MODE";
	return text;
}

const char * Screen::getOkButtonText() const
{
	static const char text[] PROGMEM = "ENTER";
	return text;
}
