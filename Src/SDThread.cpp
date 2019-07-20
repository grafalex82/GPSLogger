#include "USBDebugLogger.h"
#include "SDIODriver.h"

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

void vSDThread(void *pvParameters)
{
	//initSDThread();

	while(true)
	{
		vTaskDelay(3000);

		if(sdioDriver.init())
		{
			usbDebugWrite("SD Card initialized successfully!\n");

			uint32_t prev = HAL_GetTick();
			for(uint32_t kb=0; kb<8000000; )
			{
				sdioDriver.cardRead(kb/2, readbuf, 8);
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
		else
			usbDebugWrite("Failed to initialize SD card!\n");
	}
}

