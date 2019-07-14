#include "USBDebugLogger.h"

#include <Print.h>

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_ll_gpio.h>

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


// Pins assignment
static GPIO_TypeDef * const  SD_ENABLE_PIN_PORT = GPIOC;
static const uint32_t        SD_ENABLE_PIN_NUM  = LL_GPIO_PIN_7;
static GPIO_TypeDef * const  SDIO_D0_PIN_PORT   = GPIOC;
static const uint32_t        SDIO_D0_PIN_NUM    = LL_GPIO_PIN_8;
static GPIO_TypeDef * const  SDIO_D1_PIN_PORT   = GPIOC;
static const uint32_t        SDIO_D1_PIN_NUM    = LL_GPIO_PIN_9;
static GPIO_TypeDef * const  SDIO_D2_PIN_PORT   = GPIOC;
static const uint32_t        SDIO_D2_PIN_NUM    = LL_GPIO_PIN_10;
static GPIO_TypeDef * const  SDIO_D3_PIN_PORT   = GPIOC;
static const uint32_t        SDIO_D3_PIN_NUM    = LL_GPIO_PIN_11;
static GPIO_TypeDef * const  SDIO_CK_PIN_PORT   = GPIOC;
static const uint32_t        SDIO_CK_PIN_NUM    = LL_GPIO_PIN_12;
static GPIO_TypeDef * const  SDIO_CMD_PIN_PORT   = GPIOD;
static const uint32_t        SDIO_CMD_PIN_NUM    = LL_GPIO_PIN_2;


// SD card instance
//SdFatSPIDriver spiDriver;
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

void initSDIOPins()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SDIO_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	LL_GPIO_SetPinMode(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_MODE_OUTPUT);                    // Enable pin
	LL_GPIO_SetPinOutputType(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_SPEED_FREQ_LOW);

	//@TODO: Always SD card for now. Measure idle current vs SD card enabled/initialized. Consider removing enable transistor
	LL_GPIO_ResetOutputPin(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM);


	// Initialize SDIO pins:
	LL_GPIO_SetPinMode(SDIO_D0_PIN_PORT, SDIO_D0_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(SDIO_D0_PIN_PORT, SDIO_D0_PIN_NUM, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinSpeed(SDIO_D0_PIN_PORT, SDIO_D0_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(SDIO_D1_PIN_PORT, SDIO_D1_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(SDIO_D1_PIN_PORT, SDIO_D1_PIN_NUM, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinSpeed(SDIO_D1_PIN_PORT, SDIO_D1_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(SDIO_D2_PIN_PORT, SDIO_D2_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(SDIO_D2_PIN_PORT, SDIO_D2_PIN_NUM, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinSpeed(SDIO_D2_PIN_PORT, SDIO_D2_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(SDIO_D3_PIN_PORT, SDIO_D3_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(SDIO_D3_PIN_PORT, SDIO_D3_PIN_NUM, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinSpeed(SDIO_D3_PIN_PORT, SDIO_D3_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(SDIO_CK_PIN_PORT, SDIO_CK_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(SDIO_CK_PIN_PORT, SDIO_CK_PIN_NUM, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(SDIO_CK_PIN_PORT, SDIO_CK_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(SDIO_CMD_PIN_PORT, SDIO_CMD_PIN_NUM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(SDIO_CMD_PIN_PORT, SDIO_CMD_PIN_NUM, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(SDIO_CMD_PIN_PORT, SDIO_CMD_PIN_NUM, LL_GPIO_SPEED_FREQ_HIGH);
}

bool initSDCard()
{
	DMA_HandleTypeDef hdma_sdio;
	SD_HandleTypeDef hsd;

	usbDebugWrite("Initializing SDIO ports...\n");
	initSDIOPins();

	usbDebugWrite("Initialize SDIO...\n");

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_FALLING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 250;

	if (HAL_SD_Init(&hsd) != HAL_OK)
	{
		usbDebugWrite("Failed to initialize SDIO\n");
		return false;
	}

	usbDebugWrite("Initialize SDIO DMA...\n");
	hdma_sdio.Instance = DMA2_Channel4;
	hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
	hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_sdio.Init.Mode = DMA_NORMAL;
	hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_sdio) != HAL_OK)
	{
	  usbDebugWrite("Initialize SDIO DMA Failed\n");
	  return false;
	}

	usbDebugWrite("Initializing Card...\n");
	HAL_StatusTypeDef err = HAL_SD_InitCard(&hsd);
	usbDebugWrite("InitCard() status: %d\n", err);
	usbDebugWrite("InitCard() State: %d\n", hsd.State);
	usbDebugWrite("InitCard() error code: %04x\n", hsd.ErrorCode);

	usbDebugWrite("Setting 4-byte bus\n");
	err = HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() status: %d\n", err);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() State: %d\n", hsd.State);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() error code: %04x\n", hsd.ErrorCode);

	usbDebugWrite("Done\n");

	return true;
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


void vSDThread(void *pvParameters)
{
	//initSDThread();

	while(true)
	{
		vTaskDelay(3000);

		if(initSDCard())
		{
			//runSDMessageLoop();
			usbDebugWrite("SD Card initialized successfully!\n");
		}

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

