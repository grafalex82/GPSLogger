#include <Adafruit_SSD1306.h>

#include "CurrentPositionScreen.h"

extern Adafruit_SSD1306 display;

void CurrentPositionScreen::drawScreen()
{
	display.setFont(NULL); //TODO: Use 8x12 font to effectively use screen space
	display.setCursor(0,8);
	display.println(F("LA:  --\"--'--.--")); //TODO Print actual coordinates
	display.println(F("LO: ---\"--'--.--")); //TODO Print actual coordinates
}

CurrentPositionScreen::~CurrentPositionScreen()
{
}

const char * CurrentPositionScreen::getOkButtonText()
{
	static const char text[] PROGMEM = "POI";
	return text;
}

