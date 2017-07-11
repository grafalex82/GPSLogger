#include <Adafruit_SSD1306.h>

#include "8x12Font.h"
#include "SettingsGroupScreen.h"

extern Adafruit_SSD1306 display;

SettingsGroupScreen::SettingsGroupScreen()
{
}

void SettingsGroupScreen::drawScreen() const
{
	display.setFont(&Monospace8x12Font);
	drawCentered("Settings", 26);
}

void SettingsGroupScreen::drawCentered(const char * str, uint8_t y) const
{
	uint8_t x = display.width() / 2 - strlen(str) * 8 / 2;
	display.setCursor(x, y);
	display.print(str);
}
