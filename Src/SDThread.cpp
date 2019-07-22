#include "USBDebugLogger.h"
#include "SDIODriver.h"
#include "FreeRTOSHelpers.h"

#include <Print.h>

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

//#include "SdFatSPIDriver.h"
#include "SDThread.h"
#include "USBDebugLogger.h"


/// The SD card driver
/// @TODO: Consider having a some kind of interface here, so that SPI-based SD card could be
///        also used for backward compatibility (someone asked to run this on BluePill board
///        with SD card connected via SPI interface.
SDIODriver sdioDriver;


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

#endif


uint8_t readbuf[4096];


// SD card is a pretty slow resource. In order to let several threads work with SD card
// this file introduces a request queue. SDThread asynchronously processes read/write
// requests placed to this queue. The module also provides a few public functions to
// read or write the card in various modes - sync or async, normal or executed from an
// interrupt


// Call back indicating that SD card read/write operation completed
typedef void (*sdOperationCompletedCB)(uint8_t res, void * context);

/// Enumeration of supported operations that can be done on SD card
enum IOOperation
{
	IO_Read,
	IO_Write
};

/// A single read/write request
struct IOMsg
{
	IOOperation op;
	uint32_t lba;
	uint8_t * buf;
	uint16_t len;
	sdOperationCompletedCB cb;
	void * context;
};

// A queue for SD card read/write requests
Queue<IOMsg, 5> sdCmdQueue;


Sema readSema;

void cardReadCB(uint8_t res, void * context)
{
	readSema.give();
	//TODO: process result somehow
}

bool cardRead(uint32_t lba, uint8_t * pBuf, uint32_t blocksCount)
{
	IOMsg msg;
	msg.op = IO_Read;
	msg.lba = lba;
	msg.buf = pBuf;
	msg.len = blocksCount;
	msg.cb = cardReadCB;
	msg.context = nullptr;

	sdCmdQueue.send(msg, 100);

	return readSema.take(100);
}


void runSDQueueLoop()
{
	while(true)
	{
		IOMsg msg;
		if(sdCmdQueue.receive(&msg, 10))
		{
			//usbDebugWrite("   SD : Received IO message %d for lba %d\r\n", msg.op, msg.lba);

			switch(msg.op)
			{
				case IO_Read:
				{
					bool res = sdioDriver.cardRead(msg.lba, msg.buf, msg.len);
					msg.cb(res ? HAL_OK : HAL_ERROR, msg.context);
					break;
				}
				case IO_Write:
				{
					bool res = sdioDriver.cardWrite(msg.lba, msg.buf, msg.len);
					msg.cb(res ? HAL_OK : HAL_ERROR, msg.context);
					break;
				}
				default:
					break;
			}


			//usbDebugWrite("   SD : Operation finished for LBA %d. Sending result %d back\r\n", msg.lba, res);
		}
	}
}

void vSDThread(void *pvParameters)
{
	while(true)
	{
		vTaskDelay(3000);

		if(sdioDriver.init())
		{
			usbDebugWrite("SD Card initialized successfully!\n");

			runSDQueueLoop();
			usbDebugWrite("Failed to process SD card loop. Restarting SD card!\n");
		}
		else
			usbDebugWrite("Failed to initialize SD card!\n");
	}
}


void xSDTestThread(void *pvParameters)
{
	while(true)
	{
		vTaskDelay(5000);
		usbDebugWrite("Start reading...\n");

		uint32_t prev = HAL_GetTick();
		uint32_t kb = 0;
		for(uint32_t s=0; s<8000000; s+=8)
		{
			cardRead(s, readbuf, 8);
			kb += 4;

			uint32_t cur = HAL_GetTick();
			if(cur-prev >= 1000)
			{
				usbDebugWrite("Read %d kb\n", kb);
				kb = 0;
				prev = HAL_GetTick();
			}

		}
	}
}
