#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_spi.h>
#include <stm32f1xx_ll_gpio.h>

#include "SPIDisplayDriver.h"

// A display driver instance
SPIDisplayDriver displayDriver;


SPIDisplayDriver::SPIDisplayDriver()
{
	xSema = NULL;
}

void SPIDisplayDriver::begin()
{
	// Init sync object
	portDISABLE_INTERRUPTS();
	xSema = xSemaphoreCreateBinary();
	portENABLE_INTERRUPTS();

	// Enable clocking of corresponding periperhal
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();


	// Init pins
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);				// D/C pin
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);			// MOSI: AF PP
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);				// MISO: Input Floating

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);			// SCK: AF PP
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);

	// Init SPI peripheral for 9 MBit/s
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

	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

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
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void SPIDisplayDriver::sendCommand(uint8_t cmd)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
	HAL_SPI_Transmit(&spiHandle, &cmd, 1, 10);
}

void SPIDisplayDriver::sendData(uint8_t * data, size_t size)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
	//HAL_SPI_Transmit(&spiHandle, data, size, 10);

	// Start data transfer
	HAL_SPI_Transmit_DMA(&spiHandle, data, size);

	// Wait until transfer is completed
	xSemaphoreTake(xSema, 10);
}

void SPIDisplayDriver::dmaTransferCompletedCB()
{
	// Resume SD thread
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(xSema, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(displayDriver.getHandle().hdmatx);
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	displayDriver.dmaTransferCompletedCB();
}
