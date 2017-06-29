#include <Arduino_FreeRTOS.h>

#include <SerialUSB.h>
#define Serial SerialUSB
#include <SdFat.h>

#include "SdFatSPIDriver.h"
#include "SDThread.h"

SdFatSPIDriver spiDriver;
SdFat SD(&spiDriver);

//Sd2Card card;
//SdVolume volume;
//SdFile root;

FatFile rawDataFile;

enum SDMessageType
{
	RAW_GPS_DATA,
	CURRENT_POSITION_DATA,
	USER_WAY_POINT_DATA
};

struct RawGPSData
{
	uint8_t len;
	char rawDataBuf[maxRawGPSDataLen + 1];
};

struct SDMessage
{
	SDMessageType messageType;
	uint16_t msgIdx;

	union
	{
		RawGPSData rawData;
	};
};

QueueHandle_t sdQueue;


// To same some RAM I am using a single buffer to accumulate war GPS data and then send it to
// SD thread. Alternative would be accumulating data in a separate buffer in GPS thread, then
// assembly the message and therefore copy the buffer to message object.
SDMessage rawGPSDataBuf;
uint16_t msgIdx = 0;

char * requestRawGPSBuffer()
{
	return rawGPSDataBuf.rawData.rawDataBuf;
}

void ackRawGPSData(uint8_t len)
{
	rawGPSDataBuf.messageType = RAW_GPS_DATA;
	rawGPSDataBuf.rawData.len = len;
	rawGPSDataBuf.msgIdx = msgIdx++;

//	SerialUSB.print("Senging GPS message ");
//	SerialUSB.print(rawGPSDataBuf.msgIdx);
//	SerialUSB.print(": ");
//	SerialUSB.write(rawGPSDataBuf.rawData.rawDataBuf, rawGPSDataBuf.rawData.len);

	xQueueSend(sdQueue, &rawGPSDataBuf, 10);
}

bool initSDCard()
{
	//TODO Perhaps we should uninitialize SD card first

	SerialUSB.println("Initializing SD card...");
#if 0
	// Initialize
	spiDriver.begin(0);

	// Deselect card first
	spiDriver.deactivate();

	//We must supply at least 74 clocks with CS high
	for(int i=0; i<10; i++)
		spiDriver.send(0xff);
	vTaskDelay(5);

	// Select card
	spiDriver.activate();

	// Wait
	uint16_t q = 0;
	while (spiDriver.receive() != 0XFF);
	{
		q++;
		if(q == 0x1000)
		{
			SerialUSB.println("Failed :(");
			return false;
		}
	}

	SerialUSB.println("Works :)");
#endif //0





#if 1
	// see if the card is present and can be initialized:
	if (!SD.begin(PA4))
	{
		SerialUSB.println("Card failed, or not present");
		// don't do anything more:
		return false;
	}
	SerialUSB.println("card initialized.");

	rawDataFile.open(&SD, "RAW_GPS.TXT", O_RDWR | O_CREAT | O_AT_END | O_SYNC);
#endif
	return true;
}

void saveRawData(const SDMessage & msg)
{
//	SerialUSB.print("Dumping GPS message ");
//	SerialUSB.print(msg.msgIdx);
//	SerialUSB.print(": ");
//	SerialUSB.write(rawGPSDataBuf.rawData.rawDataBuf, rawGPSDataBuf.rawData.len);

	rawDataFile.write(rawGPSDataBuf.rawData.rawDataBuf, rawGPSDataBuf.rawData.len);
}

void runSDMessageLoop()
{
	uint16_t i=0;
	while(true)
	{
/*
		SDMessage msg;
		if(xQueueReceive(sdQueue, &msg, 50)) // TODO: fix hardcode
		{
			switch(msg.messageType)
			{
			case RAW_GPS_DATA:
				saveRawData(msg);
				break;
			default:
				break;
			}
		}
		*/

		SerialUSB.print("Writing ");
		SerialUSB.println(i);

		rawDataFile.write("Test");
		rawDataFile.printField(i, '\n');

		i++;
		vTaskDelay(1000);
	}
}

void initSDThread()
{
	// Initialize SD messages queue
	sdQueue = xQueueCreate(5, sizeof(SDMessage)); // hope 5 messages would be enough
}

void vSDThread(void *pvParameters)
{
	while(true)
	{
		vTaskDelay(2000);

		if(initSDCard())
			runSDMessageLoop();
	}
}



/*
	SerialUSB.println("Initializing card...");

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
exit:

*/
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

/*
	SerialUSB.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4)) {
	  SerialUSB.println("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	SerialUSB.println("card initialized.");

	File dataFile = SD.open("TEST.DAT", FILE_WRITE);
	if (dataFile)
	{
		SerialUSB.println("File opened");
		for(int i=0; i<100; i++)
		{
			SerialUSB.print("Printing a message #");
			SerialUSB.println(i);
			dataFile.print("Printing a message #");
			dataFile.println(i);
		}
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else
	{
		SerialUSB.println("error opening TEST.DAT");
	}
*/

