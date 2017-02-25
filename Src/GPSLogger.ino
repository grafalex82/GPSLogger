#include <Wire.h>
//#include <SoftwareSerial.h>
#include <TinyGPS++.h>


#include "ScreenManager.h"

TinyGPSPlus gps;
//SoftwareSerial gpsSerial(10, 11); // RX, TX

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(9600);

	initDisplay();
	initScreens();	
//	gpsSerial.begin(9600);
}



// the loop function runs over and over again forever
void loop()
{
	// TODO: Process GPS data here

	// TODO: Sleep here for 50 ms or so

	// Do what we need for current state
	drawDisplay();

	// It is time for change state
	processState();
}

