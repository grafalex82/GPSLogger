#include "SDIODriver.h"
#include "USBDebugLogger.h"

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_ll_gpio.h>

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


SDIODriver::SDIODriver()
{
	xSema = xSemaphoreCreateBinaryStatic(&xSemaBuffer);
}

void SDIODriver::initSDIOPins()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	LL_GPIO_SetPinMode(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_MODE_OUTPUT);                    // Enable pin
	LL_GPIO_SetPinOutputType(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(SD_ENABLE_PIN_PORT, SD_ENABLE_PIN_NUM, LL_GPIO_SPEED_FREQ_LOW);

	//@TODO: Always enable SD card for now. Measure idle current vs SD card enabled/initialized. Consider removing enable transistor
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

bool SDIODriver::initSDIO()
{
	__HAL_RCC_SDIO_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	sdHandle.Instance = SDIO;
	sdHandle.Init.ClockEdge = SDIO_CLOCK_EDGE_FALLING;
	sdHandle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	sdHandle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	sdHandle.Init.BusWide = SDIO_BUS_WIDE_1B;
	sdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	sdHandle.Init.ClockDiv = 0;
	if(HAL_SD_Init(&sdHandle) != HAL_OK)
		return false;

	initReadDMA();
	initWriteDMA();

	__HAL_LINKDMA(&sdHandle, hdmarx, dmaReadHandle);
	__HAL_LINKDMA(&sdHandle, hdmatx, dmaWriteHandle);

	HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
	HAL_NVIC_SetPriority(SDIO_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(SDIO_IRQn);

	return true;
}

bool SDIODriver::initReadDMA()
{
	dmaReadHandle.Instance = DMA2_Channel4;
	dmaReadHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dmaReadHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	dmaReadHandle.Init.MemInc = DMA_MINC_ENABLE;
	dmaReadHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dmaReadHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dmaReadHandle.Init.Mode = DMA_NORMAL;
	dmaReadHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
	return HAL_DMA_Init(&dmaReadHandle) == HAL_OK;
}

bool SDIODriver::initWriteDMA()
{
	dmaWriteHandle.Instance = DMA2_Channel4;
	dmaWriteHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dmaWriteHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	dmaWriteHandle.Init.MemInc = DMA_MINC_ENABLE;
	dmaWriteHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dmaWriteHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dmaWriteHandle.Init.Mode = DMA_NORMAL;
	dmaWriteHandle.Init.Priority = DMA_PRIORITY_MEDIUM;

	return HAL_DMA_Init(&dmaWriteHandle) == HAL_OK;
}

bool SDIODriver::initCard()
{
	HAL_StatusTypeDef err = HAL_SD_InitCard(&sdHandle);
	if (err != HAL_OK)
		return false;

	err = HAL_SD_ConfigWideBusOperation(&sdHandle, SDIO_BUS_WIDE_4B);
	if (err != HAL_OK)
		return false;

	return true;
}

bool SDIODriver::init()
{
	usbDebugWrite("Initializing SDIO ports...\n");
	initSDIOPins();


	usbDebugWrite("Initialize SDIO...\n");
	if(!initSDIO())
	{
		usbDebugWrite("Failed to initialize SDIO\n");
		return false;
	}

	if(!initCard())
	{
	  usbDebugWrite("Initialize SD Card failed\n");
	  return false;
	}

	return true;
}

bool SDIODriver::cardRead(uint32_t lba, uint8_t * pBuf, uint32_t blocksCount)
{
	// @TODO Do not call this on controllers that have separate read and write DMA
	// channels for SDIO.
	initReadDMA();

	// Start read operation
	curDMAHandle = sdHandle.hdmarx;
	HAL_StatusTypeDef err = HAL_SD_ReadBlocks_DMA(&sdHandle, pBuf, lba, blocksCount);
	if(err != HAL_OK)
		return false;

	// Wait until transfer is completed
	if(xSemaphoreTake(xSema, 100) != pdTRUE)
		return false;

	return true;
}

bool SDIODriver::cardWrite(uint32_t lba, const uint8_t * pBuf, uint32_t blocksCount)
{
	// @TODO Do not call this on controllers that have separate read and write DMA
	// channels for SDIO.
	initWriteDMA();

	// Start read operation
	curDMAHandle = sdHandle.hdmatx;
	HAL_StatusTypeDef err = HAL_SD_WriteBlocks_DMA(&sdHandle, const_cast<uint8_t *>(pBuf), lba, blocksCount);
	if(err != HAL_OK)
		return false;

	// Wait until transfer is completed
	if(xSemaphoreTake(xSema, 100) != pdTRUE)
		return false;

	return true;
}

void SDIODriver::dmaTransferCompletedCB()
{
	// Resume SD thread
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(xSema, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

extern SDIODriver sdioDriver;


extern "C" void DMA2_Channel4_5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(sdioDriver.getDMAHandle());
}

extern "C" void SDIO_IRQHandler(void)
{
	HAL_SD_IRQHandler(sdioDriver.getHandle());
}

extern "C" void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	sdioDriver.dmaTransferCompletedCB();
}

extern "C" void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	sdioDriver.dmaTransferCompletedCB();
}

extern "C" void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
	sdioDriver.dmaTransferCompletedCB();
}

extern "C" void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
	sdioDriver.dmaTransferCompletedCB();
}
