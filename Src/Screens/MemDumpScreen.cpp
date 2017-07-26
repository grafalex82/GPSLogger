#include <Adafruit_SSD1306.h>

#include "MemDumpScreen.h"
#include "ScreenManager.h"
#include "PrintUtils.h"

extern Adafruit_SSD1306 display;

MemDumpScreen::MemDumpScreen()
{
	displayAddr = (uint8_t*)0x40005C00;
}

void MemDumpScreen::drawScreen() const
{
	display.setFont(NULL);

	display.setCursor(0,8);
	printToDisplay("Addr: %08x", displayAddr);

	display.setCursor(0,16);
	uint8_t * addr = displayAddr;
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x", *(addr++));

	display.setCursor(0,24);
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x ", *(addr++));
	printToDisplay("%02x", *(addr++));
}

void MemDumpScreen::onSelButton()
{
	displayAddr += 8;
}

void MemDumpScreen::onOkButton()
{
	backToParentScreen();
}

const char * MemDumpScreen::getSelButtonText() const
{
	static const char text[] = "ADDR";
	return text;
}

const char * MemDumpScreen::getOkButtonText() const
{
	static const char text[] = "BACK";
	return text;
}
