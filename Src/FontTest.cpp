#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>
#include <gfxfont.h>

#include <fonts/FreeMono12pt7b.h>
#include <fonts/FreeMono18pt7b.h>
#include <fonts/FreeMono24pt7b.h>
#include <fonts/FreeMono9pt7b.h>
#include <fonts/FreeMonoBold12pt7b.h>
#include <fonts/FreeMonoBold18pt7b.h>
#include <fonts/FreeMonoBold24pt7b.h>
#include <fonts/FreeMonoBold9pt7b.h>
#include <fonts/FreeMonoBoldOblique12pt7b.h>
#include <fonts/FreeMonoBoldOblique18pt7b.h>
#include <fonts/FreeMonoBoldOblique24pt7b.h>
#include <fonts/FreeMonoBoldOblique9pt7b.h>
#include <fonts/FreeMonoOblique12pt7b.h>
#include <fonts/FreeMonoOblique18pt7b.h>
#include <fonts/FreeMonoOblique24pt7b.h>
#include <fonts/FreeMonoOblique9pt7b.h>
#include <fonts/FreeSans12pt7b.h>
#include <fonts/FreeSans18pt7b.h>
#include <fonts/FreeSans24pt7b.h>
#include <fonts/FreeSans9pt7b.h>
#include <fonts/FreeSansBold12pt7b.h>
#include <fonts/FreeSansBold18pt7b.h>
#include <fonts/FreeSansBold24pt7b.h>
#include <fonts/FreeSansBold9pt7b.h>
#include <fonts/FreeSansBoldOblique12pt7b.h>
#include <fonts/FreeSansBoldOblique18pt7b.h>
#include <fonts/FreeSansBoldOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique9pt7b.h>
#include <fonts/FreeSansOblique12pt7b.h>
#include <fonts/FreeSansOblique18pt7b.h>
#include <fonts/FreeSansOblique24pt7b.h>
#include <fonts/FreeSansOblique9pt7b.h>
#include <fonts/FreeSerif12pt7b.h>
#include <fonts/FreeSerif18pt7b.h>
#include <fonts/FreeSerif24pt7b.h>
#include <fonts/FreeSerif9pt7b.h>
#include <fonts/FreeSerifBold12pt7b.h>
#include <fonts/FreeSerifBold18pt7b.h>
#include <fonts/FreeSerifBold24pt7b.h>
#include <fonts/FreeSerifBold9pt7b.h>
#include <fonts/FreeSerifBoldItalic12pt7b.h>
#include <fonts/FreeSerifBoldItalic18pt7b.h>
#include <fonts/FreeSerifBoldItalic24pt7b.h>
#include <fonts/FreeSerifBoldItalic9pt7b.h>
#include <fonts/FreeSerifItalic12pt7b.h>
#include <fonts/FreeSerifItalic18pt7b.h>
#include <fonts/FreeSerifItalic24pt7b.h>
#include <fonts/FreeSerifItalic9pt7b.h>
#include <fonts/TomThumb.h>


struct font_and_name
{
	const char * name;
	const GFXfont * font;
};

#define FONT(name) {#name, &name}

const font_and_name fonts[] = {
	/*
	FONT(FreeMono12pt7b),
	*/
	FONT(FreeMono18pt7b),
//	FONT(FreeMono24pt7b),
	/*
	FONT(FreeMono9pt7b),
	FONT(FreeMonoBold12pt7b),
	FONT(FreeMonoBold18pt7b),
	FONT(FreeMonoBold24pt7b),
	FONT(FreeMonoBold9pt7b),
	FONT(FreeMonoBoldOblique12pt7b),
	FONT(FreeMonoBoldOblique18pt7b),
	FONT(FreeMonoBoldOblique24pt7b),
	FONT(FreeMonoBoldOblique9pt7b),
	FONT(FreeMonoOblique12pt7b),
	FONT(FreeMonoOblique18pt7b),
	FONT(FreeMonoOblique24pt7b),
	FONT(FreeMonoOblique9pt7b),
	FONT(FreeSans12pt7b),
	*/	
	FONT(FreeSans18pt7b),
	/*
	FONT(FreeSans24pt7b),
	FONT(FreeSans9pt7b),
	FONT(FreeSansBold12pt7b),
	FONT(FreeSansBold18pt7b),
	FONT(FreeSansBold24pt7b),
	FONT(FreeSansBold9pt7b),
	FONT(FreeSansBoldOblique12pt7b),
	FONT(FreeSansBoldOblique18pt7b),
	FONT(FreeSansBoldOblique24pt7b),
	FONT(FreeSansBoldOblique9pt7b),
	FONT(FreeSansOblique12pt7b),
	FONT(FreeSansOblique18pt7b),
	FONT(FreeSansOblique24pt7b),
	FONT(FreeSansOblique9pt7b),
	FONT(FreeSerif12pt7b),
	*/
	FONT(FreeSerif18pt7b),
//	FONT(FreeSerif24pt7b),
	/*
	FONT(FreeSerif9pt7b),
	FONT(FreeSerifBold12pt7b),
	FONT(FreeSerifBold18pt7b),
	FONT(FreeSerifBold24pt7b),
	FONT(FreeSerifBold9pt7b),
	FONT(FreeSerifBoldItalic12pt7b),
	FONT(FreeSerifBoldItalic18pt7b),
	FONT(FreeSerifBoldItalic24pt7b),
	FONT(FreeSerifBoldItalic9pt7b),
	FONT(FreeSerifItalic12pt7b),
	FONT(FreeSerifItalic18pt7b),
	FONT(FreeSerifItalic24pt7b),
	FONT(FreeSerifItalic9pt7b),
	FONT(TomThumb)*/
};
const unsigned int fonts_count = sizeof(fonts) / sizeof(font_and_name);
unsigned int current_font = 0;

extern Adafruit_SSD1306 display;

void RunFontTest()
{
	display.clearDisplay();
	//	screens[current_screen]->drawScreen();
	display.setCursor(0,30);
	display.setFont(fonts[current_font].font);
	display.print("12:34:56");
	display.setCursor(0,6);
	display.setFont(&TomThumb);
	display.print(fonts[current_font].name);
	display.display();
}

void SwitchToNextFont()
{
	current_font = (current_font + 1) % fonts_count;
}

