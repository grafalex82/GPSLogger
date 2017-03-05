#include <Adafruit_SSD1306_STM32.h>
#include "8x12Font.h"

#include "ScreenManager.h"
#include "CurrentPositionScreen.h"

extern Adafruit_SSD1306 display;

void CurrentPositionScreen::drawScreen()
{
	display.setFont(&Monospace8x12Font); //TODO: Use 8x12 font to effectively use screen space
	display.setCursor(0,20);
	display.print(F("LA: --\"--'--.--")); //TODO Print actual coordinates
	display.setCursor(0,32);
	display.print(F("LO:---\"--'--.--")); //TODO Print actual coordinates
}

const char * CurrentPositionScreen::getOkButtonText()
{
	static const char text[] PROGMEM = "POI";
	return text;
}

void CurrentPositionScreen::onOkButton()
{
	static const char text[] PROGMEM = "Waypoint Saved!";
	messageBox(text);
}
