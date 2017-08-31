#include <stdint.h>
#include <stdlib.h>

#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_gpio.h>
#include <stm32f1xx_hal_spi.h>

#include <Arduino_FreeRTOS.h>
#include <SPI.h> // Should not be included, but SPISettings is defined there

#include "SdFatSPIDriver.h"
#include "USBDebugLogger.h"

//#define USB_DEBUG
uint8_t debugEnabled = 0;

const size_t DMA_TRESHOLD = 16;

#include "USBDebugLogger.h"

SdFatSPIDriver::SdFatSPIDriver()
{
}

void SdFatSPIDriver::begin(uint8_t chipSelectPin)
{
	// Ignore passed CS pin - This driver works with predefined one
	(void)chipSelectPin;

	// Initialize GPS Thread handle
	portDISABLE_INTERRUPTS();
	xSema = xSemaphoreCreateBinary();
	portENABLE_INTERRUPTS();

	// Enable clocking of corresponding periperhal
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();


	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);

	// Init pins
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);			// MOSI: AF PP
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);				// MISO: Input Floating

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);			// SCK: AF PP
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);				// CS
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_MEDIUM);

	// Set CS pin High by default
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);

	// Init SPI
	spiHandle.Instance = SPI1;
	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spiHandle.Init.NSS = SPI_NSS_SOFT;
	spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
	spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	HAL_SPI_Init(&spiHandle);
	__HAL_SPI_ENABLE(&spiHandle);

	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Rx DMA channel
	dmaHandleRx.Instance = DMA1_Channel2;
	dmaHandleRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dmaHandleRx.Init.PeriphInc = DMA_PINC_DISABLE;
	dmaHandleRx.Init.MemInc = DMA_MINC_ENABLE;
	dmaHandleRx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dmaHandleRx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dmaHandleRx.Init.Mode = DMA_NORMAL;
	dmaHandleRx.Init.Priority = DMA_PRIORITY_LOW;
	HAL_DMA_Init(&dmaHandleRx);
	__HAL_LINKDMA(&spiHandle, hdmarx, dmaHandleRx);

	// Tx DMA channel
	dmaHandleTx.Instance = DMA1_Channel3;
	dmaHandleTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dmaHandleTx.Init.PeriphInc = DMA_PINC_DISABLE;
	dmaHandleTx.Init.MemInc = DMA_MINC_ENABLE;
	dmaHandleTx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dmaHandleTx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dmaHandleTx.Init.Mode = DMA_NORMAL;
	dmaHandleTx.Init.Priority = DMA_PRIORITY_LOW;
	HAL_DMA_Init(&dmaHandleTx);
	__HAL_LINKDMA(&spiHandle, hdmatx, dmaHandleTx);

	// Setup DMA interrupts
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void SdFatSPIDriver::activate()
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== activate\n");
#endif
	// No special activation needed
}


void SdFatSPIDriver::deactivate()
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== deactivate\n");
#endif
	// No special deactivation needed
}

uint8_t SdFatSPIDriver::receive()
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== receive: ");
#endif

	uint8_t buf;
	uint8_t dummy = 0xff;
	HAL_SPI_TransmitReceive(&spiHandle, &dummy, &buf, 1, 10);

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("%02x\n", buf);
#endif

	return buf;
}

uint8_t SdFatSPIDriver::receive(uint8_t* buf, size_t n)
{
	memset(buf, 0xff, n);

	// Not using DMA for short transfers
	if(n <= DMA_TRESHOLD)
	{
#ifdef USB_DEBUG
		if(debugEnabled)
		usbDebugWrite("== reading %d bytes\n", n);
#endif
		uint8_t s = HAL_SPI_TransmitReceive(&spiHandle, buf, buf, n, 10);

		if(s)
			usbDebugWrite("Failed ad #10: %d\n", s);

		return s;
	}

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== reading %d bytes over DMA\n", n);
#endif

		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
	// Start data transfer
	HAL_SPI_TransmitReceive_DMA(&spiHandle, buf, buf, n);

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("==   transfer started\n");
#endif

	// Wait until transfer is completed
	xSemaphoreTake(xSema, 100);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("==   transfer finished\n");
#endif

	return 0; // Ok status
}

void SdFatSPIDriver::send(uint8_t data)
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== send: %02x\n", data);
#endif

	HAL_SPI_Transmit(&spiHandle, &data, 1, 10);
}

void SdFatSPIDriver::send(const uint8_t* buf, size_t n)
{
	//vTaskDelay(1);
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== sending %d bytes\n", n);
#endif

	// Not using DMA for short transfers
	if(n <= DMA_TRESHOLD)
	{
		HAL_SPI_Transmit(&spiHandle, (uint8_t*)buf, n, 10);
		return;
	}

	// Start data transfer
	HAL_SPI_Transmit_DMA(&spiHandle, (uint8_t*)buf, n);

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("==   transfer started\n");
#endif

	// Wait until transfer is completed
	xSemaphoreTake(xSema, 100);

#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("==   transfer finished\n");
#endif
}

void SdFatSPIDriver::select()
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== select\n");
#endif

	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
}

void SdFatSPIDriver::setSpiSettings(const SPISettings & spiSettings)
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== setSetting\n");
#endif

	// Ignore settings - we are using same settings for all transfer
}

void SdFatSPIDriver::unselect()
{
#ifdef USB_DEBUG
	if(debugEnabled)
	usbDebugWrite("== unselect\n");
#endif

	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);
}

void SdFatSPIDriver::dmaTransferCompletedCB()
{
	// Resume SD thread
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(xSema, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

extern SdFatSPIDriver spiDriver;

extern "C" void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(spiDriver.getHandle().hdmarx);
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(spiDriver.getHandle().hdmatx);
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spiDriver.dmaTransferCompletedCB();
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spiDriver.dmaTransferCompletedCB();
}
