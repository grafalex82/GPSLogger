#include <Arduino_FreeRTOS.h>

#include "USBDevice.h"
#include "SerialUSB.h"

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"

//#include <SD.h>

//Sd2Card card;
//SdVolume volume;
//SdFile root;

void vSDThread(void *pvParameters) 
{
	/*
	vTaskDelay(2000);

	Serial.println("Initializing card...");

	//bool res = SD.begin(PA4);
	//Serial.print(card.type());
	uint8_t r = card.init(SPI_HALF_SPEED, PA4);


	Serial.print("Finished card initialization:");
	Serial.print(r);
	Serial.println("");

	if(!r)
	{
		Serial.print("Error code:");
		Serial.print(card.errorCode());
		Serial.println("");
		goto exit;
	}

	// print the type of card
	Serial.print("\nCard type: ");
	switch (card.type()) {
	  case SD_CARD_TYPE_SD1:
		Serial.println("SD1");
		break;
	  case SD_CARD_TYPE_SD2:
		Serial.println("SD2");
		break;
	  case SD_CARD_TYPE_SDHC:
		Serial.println("SDHC");
		break;
	  default:
		Serial.println("Unknown");
	}

	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if (!volume.init(card)) {
	  Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
	  return;
	}


	// print the type and size of the first FAT-type volume
	uint32_t volumesize;
	Serial.print("\nVolume type is FAT");
	Serial.println(volume.fatType(), DEC);
	Serial.println();

	volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	volumesize *= 512;                            // SD card blocks are always 512 bytes
	Serial.print("Volume size (bytes): ");
	Serial.println(volumesize);

	Serial.print("Volume size (Kbytes): ");
	volumesize /= 1024;
	Serial.println(volumesize);
	Serial.print("Volume size (Mbytes): ");
	volumesize /= 1024;
	Serial.println(volumesize);


	Serial.println("\nFiles found on the card (name, date and size in bytes): ");
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);
*/
	/*
	Serial.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4)) {
	  Serial.println("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	Serial.println("card initialized.");

	File dataFile = SD.open("8X12FONT.CPP");
	// if the file is available, write to it:
	if (dataFile)
	{
		Serial.println("File opened");
		while (dataFile.available())
		{
			Serial.write(dataFile.read());
		}
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else
	{
		Serial.println("error opening datalog.txt");
	}
exit:

*/


	for (;;)
	{
		vTaskDelay(2000);
		SeriaUSB.println("Test");
	}
}


int main(void)
{
	InitBoard();

	//SerialUSB.begin(115200);
	USBDeviceFS.reenumerate();
	USBDeviceFS.beginCDC();

	//initDisplay();
	initButtons();
	//initScreens();
	initGPS();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vSDThread, "SD Thread", 256, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
