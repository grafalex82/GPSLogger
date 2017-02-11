#include <Adafruit_SSD1306.h>

#include "CurrentPositionScreen.h"

extern Adafruit_SSD1306 display;

void CurrentPositionScreen::drawScreen()
{
  display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
}

CurrentPositionScreen::~CurrentPositionScreen()
{
}

