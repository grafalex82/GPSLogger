#include <Arduino_FreeRTOS.h>

#include "SdFatSPIDriver.h"
#include "SDThread.h"
#include "USBDebugLogger.h"

#include "Print.h"

class DummyPrinter : public Print
{
public:
	virtual size_t write(uint8_t c)
	{
		usbDebugWrite((char)c);
		return 1;
	}

} Serial;

#include <SdFat.h>


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

bool waitUntilReady()
{
	uint16_t q = 0;
	while (spiDriver.receive() != 0XFF)
	{
		q++;
		if(q == 0x1000)
		{
			usbDebugWrite("Card is not ready :(\n");
			return false;
		}
	}

	return true;
}

bool cardCommand(uint8_t command, uint32_t arg)
{

	waitUntilReady(); //wait for card to no longer be busy

	uint8_t commandSequence[] = {
		(uint8_t) (command | 0x40),
		(uint8_t) (arg >> 24),
		(uint8_t) (arg >> 16),
		(uint8_t) (arg >> 8),
		(uint8_t) (arg & 0xFF),
		0xFF };

	if (command == CMD0)
		commandSequence[5] = 0x95;
	else
	if (command == CMD8)
		commandSequence[5] = 0x87;

	spiDriver.send(commandSequence[0]);
	spiDriver.send(commandSequence[1]);
	spiDriver.send(commandSequence[2]);
	spiDriver.send(commandSequence[3]);
	spiDriver.send(commandSequence[4]);
	spiDriver.send(commandSequence[5]);

	//Data sent, now await Response
	uint8_t res = 0xFF;
	uint8_t count = 20;
	while ((res & 0x80) && count) {
		res = spiDriver.receive();
		count--;
	}

	return res;
}

bool initSDCard()
{
	//TODO Perhaps we should uninitialize SD card first

	usbDebugWrite("Initializing SD card...\n");
#if 1
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

	usbDebugWrite("Sengin CMD0...\n");

	uint8_t status;
	do
	{
		status = cardCommand(0, 0);
		usbDebugWrite("Command status %02x\n", status);
	}
	while(status != 0x01);

	return false;
#endif //0





#if 0
	// see if the card is present and can be initialized:
	if (!SD.begin(PA4))
	{
		usbDebugWrite("Card failed, or not present\n");
		// don't do anything more:
		return false;
	}
	usbDebugWrite("card initialized\n");

	rawDataFile.open(&SD, "RAW_GPS.TXT", O_RDWR | O_CREAT | O_AT_END | O_SYNC);
#endif

	return false;
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

		usbDebugWrite("Writing %d\n", i);

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
		vTaskDelay(3000);

		if(initSDCard())
			runSDMessageLoop();

		//vTaskDelay(2000);
	}
}



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

