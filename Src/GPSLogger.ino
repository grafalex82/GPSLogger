#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <SoftwareSerial.h>
#include <TinyGPS++.h>


#include "CurrentPositionScreen.h"
#include "CurrentTimeScreen.h"
#include "ScreenManager.h"
#include "TestScreen.h"

#include "FontTest.h"

Adafruit_SSD1306 display;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int SEL_BUTTON_PIN = 2;
const int OK_BUTTON_PIN = 3;


//SoftwareSerial gpsSerial(10, 11); // RX, TX

TinyGPSPlus gps;

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(9600);
	
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	
	pinMode(SEL_BUTTON_PIN, INPUT_PULLUP);
	pinMode(OK_BUTTON_PIN, INPUT_PULLUP);
	
//	gpsSerial.begin(9600);

/*	
	TestScreen * childScreen = new TestScreen(F("Parent Screen 1"));
	Screen * child = childScreen->addChildScreen(new TestScreen(F("Child Screen 1")));
	child = child->addScreen(new TestScreen(F("Child Screen 2")));
	child = child->addScreen(new TestScreen(F("Child Screen 3")));

	TestScreen * childScreen2 = new TestScreen(F("Child/Parent screen"));
	child = childScreen2->addChildScreen(new TestScreen(F("Child Screen 4")));
	child = child->addScreen(new TestScreen(F("Child Screen 5")));
	child = child->addScreen(new TestScreen(F("Child Screen 6")));

	TestScreen * childScreen3 = new TestScreen(F("Parent Screen 2"));
	child = childScreen3->addChildScreen(new TestScreen(F("Child Screen 7")));
	child = child->addScreen(new TestScreen(F("Child Screen 8")));
	child = child->addScreen(childScreen2);
	*/

	
	Screen * screen = createCurrentTimeScreen();
	setCurrentScreen(screen);
	screen = screen->addScreen(new CurrentPositionScreen());
	
/*	
	screen = screen->addScreen(new TestScreen(F("No children screen")));
	screen = screen->addScreen(childScreen);
	screen = screen->addScreen(childScreen3);
	screen = screen->addScreen(new TestScreen(F("Another empty Screen")));
	*/
}


// the loop function runs over and over again forever
void loop()
{
	/*
	if (gpsSerial.available()) {
		Serial.write(gpsSerial.read());
	}
	if (Serial.available()) {
		gpsSerial.write(Serial.read());
	}
	
	return;
	*/
//	RunFontTest();

	/*
	while (gpsSerial.available())
	{
		uint8_t c = gpsSerial.read(); 		
		gps.encode(c);
		Serial.write(c);
	}
		
	Serial.println("Chars:");
	Serial.println(gps.charsProcessed());
	Serial.println("sentences:");
	Serial.println(gps.sentencesWithFix());
	Serial.println("failed:");
	Serial.println(gps.failedChecksum());
	Serial.println("passed:");
	Serial.println(gps.passedChecksum());
	*/
		
	display.clearDisplay();
	Screen * currentScreen = getCurrentScreen();
	currentScreen->drawHeader();
	currentScreen->drawScreen();
	display.display();
	
	delay(200);
	
	if(!digitalRead(SEL_BUTTON_PIN))
	{
//		SwitchToNextFont();
		currentScreen->onSelButton();
	}

	if(!digitalRead(OK_BUTTON_PIN))
	{
		currentScreen->onOkButton();
	}
}

