#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

#include "CurrentTimeScreen.h"
#include "TimeZoneScreen.h"
#include "TimeFont.h"

extern Adafruit_SSD1306 display;
extern TinyGPSPlus gps;

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
	
	char buf[9];
	buf[0] = 0x30 + h / 10;
	buf[1] = 0x30 + h % 10;
	buf[2] = ':';
	buf[3] = 0x30 + m / 10;
	buf[4] = 0x30 + m % 10;
	buf[5] = ':';
	buf[6] = 0x30 + s / 10;
	buf[7] = 0x30 + s % 10;
	buf[8] = 0;
	
	display.setFont(&TimeFont);
	display.setCursor(0,31);
	display.print(buf);
}

CurrentTimeScreen::~CurrentTimeScreen()
{
}

Screen * createCurrentTimeScreen()
{
	TimeZoneScreen * tzScreen = new TimeZoneScreen(1, 30);
	tzScreen = tzScreen->addScreen(new TimeZoneScreen(2, 45));
	tzScreen = tzScreen->addScreen(new TimeZoneScreen(-3, 30));
	
	CurrentTimeScreen * screen = new CurrentTimeScreen();
	screen->addChildScreen(tzScreen); 
	return screen;
}
