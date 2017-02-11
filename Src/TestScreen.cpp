#include <Adafruit_SSD1306.h>

#include "TestScreen.h"

extern Adafruit_SSD1306 display;

TestScreen::TestScreen(const char * str)
{
	this->str = str;
}

TestScreen::~TestScreen()
{
}

void TestScreen::drawScreen()
{
	display.setTextColor(WHITE);
	display.setFont(NULL);
	display.setCursor(0,16);
	display.print(this->str);
}
