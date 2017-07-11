#include <Adafruit_SSD1306.h>

#include "8x12Font.h"
#include "SelectorScreen.h"
#include "ScreenManager.h"

extern Adafruit_SSD1306 display;

SelectorScreen::SelectorScreen()
{
	currentIdx = 0;
}

void SelectorScreen::drawScreen() const
{
	display.setFont(&Monospace8x12Font);
	if(currentIdx != getItemsCount())
		drawCentered(getItemText(currentIdx), 26);
	else
		drawCentered("Back", 26);
}

void SelectorScreen::drawCentered(const char * str, uint8_t y) const
{
	uint8_t x = display.width() / 2 - strlen(str) * 8 / 2;
	display.setCursor(x, y);
	display.print(str);
}

void SelectorScreen::onSelButton()
{
	currentIdx++;
	currentIdx %= (getItemsCount() + 1); // +1 to account 'Back' value
}

void SelectorScreen::onOkButton()
{
	// Perform an action associated with selected item
	if(currentIdx < getItemsCount())
		applySelection(currentIdx);
	
	// Reset the index so user starts from the beginning next time
	currentIdx = 0;	
	
	// Back to previous screen
	backToParentScreen();
}

const char * SelectorScreen::getSelButtonText() const
{
	static const char text[] = "NEXT";
	return text;
}

const char * SelectorScreen::getOkButtonText() const
{
	static const char text[] = "OK";
	return text;
}
