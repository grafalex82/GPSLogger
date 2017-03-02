#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"

extern Adafruit_SSD1306 display;
extern TinyGPSPlus gps;

TimeZoneScreen timeZoneScreen; //TODO Move it to CurrentTimeScreen class

void CurrentTimeScreen::drawScreen()
{
	int h = 12;
	int m = 34;
	int s = 56;
	
	/*
	int h = gps.time.hour();
	int m = gps.time.minute();
	int s = gps.time.second();
	
	Serial.println("Hour:");
	Serial.println(h);
	Serial.println("Minute:");
	Serial.println(m);
	Serial.println("second:");
	Serial.println(s);
	Serial.println("Valid:");
	Serial.println(gps.time.isValid());
	Serial.println("Age:");
	Serial.println(gps.time.age());
	*/
	
	char buf[10];
	buf[0] = ';';  // '~' symbol
	buf[1] = 0x30 + h / 10;
	buf[2] = 0x30 + h % 10;
	buf[3] = ':';
	buf[4] = 0x30 + m / 10;
	buf[5] = 0x30 + m % 10;
	buf[6] = ':';
	buf[7] = 0x30 + s / 10;
	buf[8] = 0x30 + s % 10;
	buf[9] = 0;
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);
}

CurrentTimeScreen::CurrentTimeScreen()
{
	addChildScreen(&timeZoneScreen);
}

CurrentTimeScreen::~CurrentTimeScreen()
{
}


