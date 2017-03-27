#include <Adafruit_SSD1306_STM32.h>

#include "8x12Font.h"
#include "SelectorScreen.h"
#include "ScreenManager.h"

extern Adafruit_SSD1306 display;

SelectorScreen::SelectorScreen(SelectorRecord * recs, uint8 cnt)
{
	records = recs;
	count = cnt;
	currentIdx = 0;
}

void SelectorScreen::drawScreen() const
{
	display.setFont(&Monospace8x12Font);
	if(currentIdx != count)
		drawCentered(records[currentIdx].valueStr, 26);
	else
		drawCentered("Back", 26);
}

void SelectorScreen::drawCentered(const char * str, uint8 y) const
{
	uint8 x = display.width() / 2 - strlen(str) * 8 / 2;
	display.setCursor(x, y);
	display.print(str);
}

void SelectorScreen::onSelButton()
{
	currentIdx++;
	currentIdx %= (count + 1); // +1 to account 'Back' value
	
	Serial.print("Selecting item ");
	Serial.println(currentIdx);
}

void SelectorScreen::onOkButton()
{
	if(currentIdx < count && records[currentIdx].acceptFunc)
		records[currentIdx].acceptFunc();
		
	backToParentScreen();
}

const char * SelectorScreen::getSelButtonText() const
{
	static const char text[] PROGMEM = "NEXT";
	return text;
}

const char * SelectorScreen::getOkButtonText() const
{
	static const char text[] PROGMEM = "OK";
	return text;
}
