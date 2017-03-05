#include <Adafruit_SSD1306_STM32.h>
#include <TinyGPS++.h>


#include "ScreenManager.h"

//TinyGPSPlus gps;

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(115200);

	initDisplay();
	initScreens();	
	//gpsSerial.begin(9600);
}

// the loop function runs over and over again forever
void loop()
{
	/*
	if (Serial.available()) {
		gpsSerial.write(Serial.read());
	}*/

	// TODO: Process GPS data here

	// TODO: Sleep here for 50 ms or so

	// Do what we need for current state
	drawDisplay();

	// It is time for change state
	processState();
}

