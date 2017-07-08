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

SdFatSPIDriver::SdFatSPIDriver()
{
}

//void SdFatSPIDriver::activate();
void SdFatSPIDriver::begin(uint8_t chipSelectPin)
{
	// Ignore passed CS pin - This driver works with predefined one
	(void)chipSelectPin;

	// Initialize GPS Thread handle
	xSDThread = xTaskGetCurrentTaskHandle();

	// Enable clocking of corresponding periperhal
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	// Init pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;	//MOSI & SCK
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;				//MISO
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4;				//CS
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Set CS pin High by default
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	// Init SPI
	spiHandle.Instance = SPI1;
	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spiHandle.Init.NSS = SPI_NSS_SOFT;
	spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
	spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spiHandle.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&spiHandle);
	__HAL_SPI_ENABLE(&spiHandle);
}

void SdFatSPIDriver::activate()
{
#ifdef USB_DEBUG
	usbDebugWrite("== activate\n");
#endif
	// No special activation needed
}


void SdFatSPIDriver::deactivate()
{
#ifdef USB_DEBUG
	usbDebugWrite("== deactivate\n");
#endif
	// No special deactivation needed
}

uint8_t SdFatSPIDriver::receive()
{
#ifdef USB_DEBUG
	usbDebugWrite("== receive: ");
#endif

	uint8_t buf;
	uint8_t dummy = 0xff;
	HAL_SPI_TransmitReceive(&spiHandle, &dummy, &buf, 1, 10);

#ifdef USB_DEBUG
	usbDebugWrite("%02x\n", buf);
#endif

	return buf;
}

uint8_t SdFatSPIDriver::receive(uint8_t* buf, size_t n)
{
#ifdef USB_DEBUG
	usbDebugWrite("== reading %d bytes\n", n);
#endif

	// TODO: Receive via DMA here
	HAL_SPI_Receive(&spiHandle, buf, n, 10);
	return 0;
}

void SdFatSPIDriver::send(uint8_t data)
{
#ifdef USB_DEBUG
	usbDebugWrite("== send: %02x\n", data);
#endif

	HAL_SPI_Transmit(&spiHandle, &data, 1, 10);
}

void SdFatSPIDriver::send(const uint8_t* buf, size_t n)
{
#ifdef USB_DEBUG
	usbDebugWrite("== sending %d bytes\n", n);
#endif

	// TODO: Transmit over DMA here
	HAL_SPI_Transmit(&spiHandle, (uint8_t*)buf, n, 10);
}

void SdFatSPIDriver::select()
{
#ifdef USB_DEBUG
	usbDebugWrite("== select\n");
#endif

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void SdFatSPIDriver::setSpiSettings(const SPISettings & spiSettings)
{
#ifdef USB_DEBUG
	usbDebugWrite("== setSetting\n");
#endif

	// Ignore settings - we are using same settings for all transfer
}

void SdFatSPIDriver::unselect()
{
#ifdef USB_DEBUG
	usbDebugWrite("== unselect\n");
#endif

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
