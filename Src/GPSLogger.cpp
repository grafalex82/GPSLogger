#include <Arduino_FreeRTOS.h>

#include "USBDevice.h"
#include "SerialUSB.h"

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"

#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;

void vSDThread(void *pvParameters) 
{
	vTaskDelay(2000);

	SerialUSB.println("Initializing card...");

	//bool res = SD.begin(PA4);
	//SerialUSB.print(card.type());
	uint8_t r = card.init(SPI_HALF_SPEED, PA4);


	SerialUSB.print("Finished card initialization:");
	SerialUSB.print(r);
	SerialUSB.println("");

	if(!r)
	{
		SerialUSB.print("Error code:");
		SerialUSB.print(card.errorCode());
		SerialUSB.println("");
		goto exit;
	}

	// print the type of card
	SerialUSB.print("\nCard type: ");
	switch (card.type()) {
	  case SD_CARD_TYPE_SD1:
		SerialUSB.println("SD1");
		break;
	  case SD_CARD_TYPE_SD2:
		SerialUSB.println("SD2");
		break;
	  case SD_CARD_TYPE_SDHC:
		SerialUSB.println("SDHC");
		break;
	  default:
		SerialUSB.println("Unknown");
	}

	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if (!volume.init(card)) {
	  SerialUSB.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
	  return;
	}


	// print the type and size of the first FAT-type volume
	uint32_t volumesize;
	SerialUSB.print("\nVolume type is FAT");
	SerialUSB.println(volume.fatType(), DEC);
	SerialUSB.println();

	volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	volumesize *= 512;                            // SD card blocks are always 512 bytes
	SerialUSB.print("Volume size (bytes): ");
	SerialUSB.println(volumesize);

	SerialUSB.print("Volume size (Kbytes): ");
	volumesize /= 1024;
	SerialUSB.println(volumesize);
	SerialUSB.print("Volume size (Mbytes): ");
	volumesize /= 1024;
	SerialUSB.println(volumesize);


	SerialUSB.println("\nFiles found on the card (name, date and size in bytes): ");
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);

	/*
	SerialUSB.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4)) {
	  SerialUSB.println("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	SerialUSB.println("card initialized.");

	File dataFile = SD.open("8X12FONT.CPP");
	// if the file is available, write to it:
	if (dataFile)
	{
		SerialUSB.println("File opened");
		while (dataFile.available())
		{
			SerialUSB.write(dataFile.read());
		}
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else
	{
		SerialUSB.println("error opening datalog.txt");
	}

*/

exit:

	for (;;)
	{
		vTaskDelay(2000);
		SerialUSB.println("Test");
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
