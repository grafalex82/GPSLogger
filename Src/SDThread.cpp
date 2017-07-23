#include <Print.h>
#include "USBDebugLogger.h"


// SdFat needs Serial for its interface. Provide our one
// (must be declared before including SdFat.h)
class UsbDebugSerial : public Print
{
	virtual size_t write(uint8_t c)
	{
		usbDebugWrite((char)c);
		return 1;
	}
} Serial;

//#include <SdFat.h>
#include <Arduino_FreeRTOS.h>

#include "SdFatSPIDriver.h"
#include "SDThread.h"
#include "USBDebugLogger.h"

#include "Print.h"

// SD card instance
SdFatSPIDriver spiDriver;
//SdFat SD(&spiDriver);

// Files we are working fith
//FatFile rawDataFile;
//FatFile bulkFile;

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


// To same some RAM I am using a single buffer to accumulate raw GPS data and then send it to
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

#if 0
bool initSDCard()
{
	//TODO Perhaps we should uninitialize SD card first

	usbDebugWrite("Initializing SD card...\n");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4))
	{
		usbDebugWrite("Card failed, or not present\n");
		// don't do anything more:
		return false;
	}
	usbDebugWrite("card initialized.\n");

	//rawDataFile.open(&SD, "RAW_GPS.TXT", O_RDWR | O_CREAT | O_AT_END | O_SYNC);
	//bulkFile.open(&SD, "bulk_2.dat", O_RDWR | O_CREAT | O_AT_END | O_SYNC);
	if(!bulkFile.open(&SD, "bulk.dat", O_READ))
		usbDebugWrite("Filed to open file\n");

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

uint8_t sd_buf[512];

void runSDMessageLoop()
{
	for(uint16_t q = 0; q<512; q++)
		sd_buf[q] = q & 0xff;

	uint32_t i=0;
	uint32_t prev = HAL_GetTick();
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

//		usbDebugWrite("Writing block #%d\n", i);

		//uint32_t t1 = HAL_GetTick();
		bulkFile.read(sd_buf, 512);
		//uint32_t t2 = HAL_GetTick();
		bulkFile.read(sd_buf, 512);
		//uint32_t t3 = HAL_GetTick();

//		usbDebugWrite("1kb write time: %d, %d\n", t2-t1, t3-t2);

		i++;

		uint32_t cur = HAL_GetTick();
		if(cur-prev >= 1000)
		{
			usbDebugWrite("Written %d kb\n", i);
			i = 0;

//			vTaskDelay(1000);
			prev = HAL_GetTick();
		}
	}
}

void initSDThread()
{
	// Initialize SD messages queue
	sdQueue = xQueueCreate(5, sizeof(SDMessage)); // hope 5 messages would be enough
}

void vSDThread(void *pvParameters)
{
	initSDThread();

	while(true)
	{
		vTaskDelay(3000);

		if(initSDCard())
			runSDMessageLoop();

		//vTaskDelay(2000);
	}
}

#endif

/*
	usbDebugWrite("Initializing card...");

	uint8_t r = card.init(SPI_HALF_SPEED, PA4);


	SerialUSB.print("Finished card initialization:");
	SerialUSB.print(r);
	usbDebugWrite("");

	if(!r)
	{
		SerialUSB.print("Error code:");
		SerialUSB.print(card.errorCode());
		usbDebugWrite("");
		goto exit;
	}

	// print the type of card
	SerialUSB.print("\nCard type: ");
	switch (card.type()) {
	  case SD_CARD_TYPE_SD1:
		usbDebugWrite("SD1");
		break;
	  case SD_CARD_TYPE_SD2:
		usbDebugWrite("SD2");
		break;
	  case SD_CARD_TYPE_SDHC:
		usbDebugWrite("SDHC");
		break;
	  default:
		usbDebugWrite("Unknown");
	}

	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if (!volume.init(card)) {
	  usbDebugWrite("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
	  return;
	}


	// print the type and size of the first FAT-type volume
	uint32_t volumesize;
	SerialUSB.print("\nVolume type is FAT");
	usbDebugWrite(volume.fatType(), DEC);
	usbDebugWrite();

	volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	volumesize *= 512;                            // SD card blocks are always 512 bytes
	SerialUSB.print("Volume size (bytes): ");
	usbDebugWrite(volumesize);

	SerialUSB.print("Volume size (Kbytes): ");
	volumesize /= 1024;
	usbDebugWrite(volumesize);
	SerialUSB.print("Volume size (Mbytes): ");
	volumesize /= 1024;
	usbDebugWrite(volumesize);


	usbDebugWrite("\nFiles found on the card (name, date and size in bytes): ");
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);
exit:

*/
/*
	SerialUSB.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4)) {
	  usbDebugWrite("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	usbDebugWrite("card initialized.");

	File dataFile = SD.open("8X12FONT.CPP");
	// if the file is available, write to it:
	if (dataFile)
	{
		usbDebugWrite("File opened");
		while (dataFile.available())
		{
			SerialUSB.write(dataFile.read());
		}
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else
	{
		usbDebugWrite("error opening datalog.txt");
	}
*/

/*
	SerialUSB.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(PA4)) {
	  usbDebugWrite("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	usbDebugWrite("card initialized.");

	File dataFile = SD.open("TEST.DAT", FILE_WRITE);
	if (dataFile)
	{
		usbDebugWrite("File opened");
		for(int i=0; i<100; i++)
		{
			SerialUSB.print("Printing a message #");
			usbDebugWrite(i);
			dataFile.print("Printing a message #");
			dataFile.println(i);
		}
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else
	{
		usbDebugWrite("error opening TEST.DAT");
	}
*/

