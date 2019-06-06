#include <Arduino_FreeRTOS.h>

#include "Screens/ScreenManager.h"
#include "GPS/GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"
//#include "SDThread.h"
#include "USBDebugLogger.h"
//#include "SerialDebugLogger.h"
//#include "SdMscDriver.h"
#include "I2CDriver.h"

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_hal_gpio.h>
#include <stm32f1xx_hal_sd.h>

uint8_t readbuf[512];

TaskHandle_t xSema;
DMA_HandleTypeDef hdma_sdio;
SD_HandleTypeDef hsd;

void vSDIOTestTask(void *pvParameters)
{
	// Init sync object
	portDISABLE_INTERRUPTS();
	xSema = xSemaphoreCreateBinary();
	portENABLE_INTERRUPTS();

	vTaskDelay(4000);
	usbDebugWrite("Enable SDIO Power\n");

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SDIO_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();


	static GPIO_TypeDef * const		ENABLE_PIN_PORT	= GPIOC;
	static const uint32_t			ENABLE_PIN_NUM	= LL_GPIO_PIN_7;

	LL_GPIO_SetPinMode(ENABLE_PIN_PORT, ENABLE_PIN_NUM, LL_GPIO_MODE_OUTPUT);					// Enable pin
	LL_GPIO_SetPinOutputType(ENABLE_PIN_PORT, ENABLE_PIN_NUM, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(ENABLE_PIN_PORT, ENABLE_PIN_NUM, LL_GPIO_SPEED_FREQ_LOW);

	//@TODO: Always enable GPS UART for now. Consider shutting down GPS on idle based on accelerometer values
	LL_GPIO_ResetOutputPin(ENABLE_PIN_PORT, ENABLE_PIN_NUM);

	vTaskDelay(1000);
	usbDebugWrite("Initializing SDIO\n");

	/**SDIO GPIO Configuration
	PC8     ------> SDIO_D0
	PC9     ------> SDIO_D1
	PC10     ------> SDIO_D2
	PC11     ------> SDIO_D3
	PC12     ------> SDIO_CK
	PD2     ------> SDIO_CMD
	*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	usbDebugWrite("Initialize SDIO\n");

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_FALLING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 250;

	if (HAL_SD_Init(&hsd) != HAL_OK)
		usbDebugWrite("Failed to initialize SDIO\n");


	usbDebugWrite("Initialize SDIO DMA\n");
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
	}



	HAL_StatusTypeDef err;

	usbDebugWrite("Init Card\n");
	err = HAL_SD_InitCard(&hsd);
	usbDebugWrite("InitCard() status: %d\n", err);
	usbDebugWrite("InitCard() State: %d\n", hsd.State);
	usbDebugWrite("InitCard() error code: %04x\n", hsd.ErrorCode);

	usbDebugWrite("Setting 4-byte bus\n");
	err = HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() status: %d\n", err);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() State: %d\n", hsd.State);
	usbDebugWrite("HAL_SD_ConfigWideBusOperation() error code: %04x\n", hsd.ErrorCode);

	usbDebugWrite("Done\n");


	__HAL_LINKDMA(&hsd, hdmarx, hdma_sdio);
	__HAL_LINKDMA(&hsd, hdmatx, hdma_sdio);

	HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
	HAL_NVIC_SetPriority(SDIO_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(SDIO_IRQn);

	while(1)
	{
		usbDebugWrite("CardType: %04x\n", hsd.SdCard.CardType);
		usbDebugWrite("CardVersion: %04x\n", hsd.SdCard.CardVersion);
		usbDebugWrite("Class: %02x\n", hsd.SdCard.Class);
		usbDebugWrite("RelCardAdd: %02x\n", hsd.SdCard.RelCardAdd);
		usbDebugWrite("BlockNbr: %04x\n", hsd.SdCard.BlockNbr);
		usbDebugWrite("BlockSize: %04x\n", hsd.SdCard.BlockSize);
		usbDebugWrite("LogBlockNbr: %04x\n", hsd.SdCard.LogBlockNbr);
		usbDebugWrite("LgoBlockSize: %04x\n", hsd.SdCard.LogBlockSize);

		usbDebugWrite("State: %02x\n", HAL_SD_GetCardState(&hsd));

		usbDebugWrite("CID: %08x %08x %08x %08x\n", hsd.CID[0], hsd.CID[1], hsd.CID[2], hsd.CID[3]);
		HAL_SD_CardCIDTypeDef cid;
		HAL_SD_GetCardCID(&hsd, &cid);
		usbDebugWrite("CID: ManufacturerID: %02x\n", cid.ManufacturerID);
		usbDebugWrite("CID: OEM_AppliID: %04x\n", cid.OEM_AppliID);
		usbDebugWrite("CID: ProdName1: %08x\n", cid.ProdName1);
		usbDebugWrite("CID: ProdName2: %02x\n", cid.ProdName2);
		usbDebugWrite("CID: ProdRev: %02x\n", cid.ProdRev);
		usbDebugWrite("CID: ProdSN: %08x\n", cid.ProdSN);
		usbDebugWrite("CID: ManufactDate: %04x\n", cid.ManufactDate);

		HAL_SD_CardCSDTypeDef csd;
		HAL_SD_GetCardCSD(&hsd, &csd);
		usbDebugWrite("CSD: %08x %08x %08x %08x\n", hsd.CSD[0], hsd.CSD[1], hsd.CSD[2], hsd.CSD[3]);
		usbDebugWrite("CSD: CSDStruct: %02x\n", csd.CSDStruct);
		usbDebugWrite("CSD: SysSpecVersion: %02x\n", csd.SysSpecVersion);
		usbDebugWrite("CSD: TAAC: %02x\n", csd.TAAC);
		usbDebugWrite("CSD: NSAC: %02x\n", csd.NSAC);
		usbDebugWrite("CSD: MaxBusClkFrec: %02x\n", csd.MaxBusClkFrec);
		usbDebugWrite("CSD: CardComdClasses: %04x\n", csd.CardComdClasses);
		usbDebugWrite("CSD: RdBlockLen: %02x\n", csd.RdBlockLen);
		usbDebugWrite("CSD: PartBlockRead: %02x\n", csd.PartBlockRead);
		usbDebugWrite("CSD: WrBlockMisalign: %02x\n", csd.WrBlockMisalign);
		usbDebugWrite("CSD: RdBlockMisalign: %02x\n", csd.RdBlockMisalign);
		usbDebugWrite("CSD: DSRImpl: %02x\n", csd.DSRImpl);
		usbDebugWrite("CSD: DeviceSize: %08x\n", csd.DeviceSize);
		usbDebugWrite("CSD: MaxRdCurrentVDDMin: %02x\n", csd.MaxRdCurrentVDDMin);
		usbDebugWrite("CSD: MaxRdCurrentVDDMax: %02x\n", csd.MaxRdCurrentVDDMax);
		usbDebugWrite("CSD: MaxWrCurrentVDDMin: %02x\n", csd.MaxWrCurrentVDDMin);
		usbDebugWrite("CSD: MaxWrCurrentVDDMax: %02x\n", csd.MaxWrCurrentVDDMax);
		usbDebugWrite("CSD: DeviceSizeMul: %02x\n", csd.DeviceSizeMul);
		usbDebugWrite("CSD: EraseGrSize: %02x\n", csd.EraseGrSize);
		usbDebugWrite("CSD: EraseGrMul: %02x\n", csd.EraseGrMul);
		usbDebugWrite("CSD: WrProtectGrSize: %02x\n", csd.WrProtectGrSize);
		usbDebugWrite("CSD: WrProtectGrEnable: %02x\n", csd.WrProtectGrEnable);
		usbDebugWrite("CSD: ManDeflECC: %02x\n", csd.ManDeflECC);
		usbDebugWrite("CSD: WrSpeedFact: %02x\n", csd.WrSpeedFact);
		usbDebugWrite("CSD: MaxWrBlockLen: %02x\n", csd.MaxWrBlockLen);
		usbDebugWrite("CSD: WriteBlockPaPartial: %02x\n", csd.WriteBlockPaPartial);
		usbDebugWrite("CSD: ContentProtectAppli: %02x\n", csd.ContentProtectAppli);
		usbDebugWrite("CSD: FileFormatGrouop: %02x\n", csd.FileFormatGrouop);
		usbDebugWrite("CSD: CopyFlag: %02x\n", csd.CopyFlag);
		usbDebugWrite("CSD: PermWrProtect: %02x\n", csd.PermWrProtect);
		usbDebugWrite("CSD: TempWrProtect: %02x\n", csd.TempWrProtect);
		usbDebugWrite("CSD: FileFormat: %02x\n", csd.FileFormat);
		usbDebugWrite("CSD: ECC: %02x\n", csd.ECC);


		usbDebugWrite("Current State: %d\n", hsd.State);

		vTaskDelay(1000);

		for(uint32_t s=0; s<10000; s++)
		{
			err = HAL_SD_ReadBlocks(&hsd, readbuf, s, 1, 100);
			if(err != 0)
			{
				usbDebugWrite("Read status: %d\n", err);
				usbDebugWrite("ReadCard() State: %d\n", hsd.State);
				usbDebugWrite("ReadCard() error code: %04x\n", hsd.ErrorCode);
			}

//			xSemaphoreTake(xSema, 100);

			for(int i=0; i<512; i++)
			{
				usbDebugWrite("%02x ", readbuf[i]);
				if(i%16 == 15)
					usbDebugWrite("\n");
			}

			vTaskDelay(500);
		}
		vTaskDelay(100);
	}
}
/*
extern "C"
{
	void DMA2_Channel4_5_IRQHandler(void)
	{
	  HAL_DMA_IRQHandler(&hdma_sdio);
	}

	void cb(SD_HandleTypeDef *hsd)
	{
		// Resume SD thread
		BaseType_t xHigherPriorityTaskWoken;
		xSemaphoreGiveFromISR(xSema, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
	{
		cb(hsd);
	}

	void SDIO_IRQHandler(void)
	{
	  HAL_SD_IRQHandler(&hsd);
	}


	void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
	{
		cb(hsd);
	}
	void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
	{
		cb(hsd);
	}
	void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
	{
		cb(hsd);
	}
}*/

int main(void)
{
	InitBoard();
	//initDebugSerial();

	portENABLE_INTERRUPTS(); // To allow halt() use HAL_Delay()

	// Initialize SD card before initializing USB
//	if(!initSDIOThread())
//		halt(7);

	initUSB();

	initButtons();
	//initSDThread();

	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	//xTaskCreate(vSDThread, "SD Thread", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vDisplayTask, "Display Task", 768, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(xSDIOThread, "SD IO executor", 256, NULL, tskIDLE_PRIORITY + 3, NULL);
	//xTaskCreate(xSDTestThread, "SD test thread", 200, NULL, tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(vGPSTask, "GPS Task", 256, NULL, tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(vSDIOTestTask, "Test thread", 256, NULL, tskIDLE_PRIORITY + 4, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
