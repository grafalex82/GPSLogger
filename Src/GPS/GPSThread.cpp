#include <SerialUSB.h>
#include <Arduino_FreeRTOS.h>
#include <NMEAGPS.h>
#include "Streamers.h"

#include "GPSThread.h"
#include "GPSDataModel.h"

// A GPS parser
NMEAGPS gpsParser;

// Size of UART input buffer
const uint8_t gpsBufferSize = 128;

// This class handles UART interface that receive chars from GPS and stores them to a buffer
class GPS_UART
{
	// UART hardware handle
	UART_HandleTypeDef uartHandle;

	// Receive ring buffer
	uint8_t rxBuffer[gpsBufferSize];
	uint8_t lastReadIndex = 0;
	uint8_t lastReceivedIndex = 0;

	// Semaphore to sleep until whole line is received
	SemaphoreHandle_t xSemaphore = NULL;
	//StaticSemaphore_t xSemaphoreBuffer; //TODO: Implement static allocation for semaphore and other RTOS objects

public:
	void init()
	{
		// Reset pointers (just in case someone calls init() multiple times)
		lastReadIndex = 0;
		lastReceivedIndex = 0;

		// Initialize semaphore
		//xSemaphore = xSemaphoreCreateBinary();
		xSemaphore = xSemaphoreCreateCounting(100, 0);

		// Enable clocking of corresponding periperhal
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();

		// Init pins in alternate function mode
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_9; //TX pin
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10; //RX pin
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// Init
		uartHandle.Instance = USART1;
		uartHandle.Init.BaudRate = 9600;
		uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
		uartHandle.Init.StopBits = UART_STOPBITS_1;
		uartHandle.Init.Parity = UART_PARITY_NONE;
		uartHandle.Init.Mode = UART_MODE_TX_RX;
		uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
		HAL_UART_Init(&uartHandle);

		// We will be using UART interrupt to get data
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

		// We will be waiting for a single char right received right to the buffer
		HAL_UART_Receive_IT(&uartHandle, rxBuffer, 1);
	}

	// Check if byte is available
	bool available() const
	{
		return lastReadIndex != lastReceivedIndex;
	}

	// Read a symbol
	char readChar()
	{
		if (available())
			return (char)rxBuffer[lastReadIndex++ % gpsBufferSize];
		else
			return '\0';
	}

	// Wait until whole line is received
	void waitForString()
	{
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
	}

	// Helper function, returns UART handler
	inline UART_HandleTypeDef * getUartHandle()
	{
		return &uartHandle;
	}

	// Char received, prepare for next one
	inline void charReceivedCB()
	{
		if(rxBuffer[lastReceivedIndex % gpsBufferSize] == '\n')
			xSemaphoreGiveFromISR(&xSemaphore, NULL);

		lastReceivedIndex++;
		HAL_UART_Receive_IT(&uartHandle, rxBuffer + (lastReceivedIndex % gpsBufferSize), 1);
	}


} gpsUart; // An instance of UART handler


// Forward UART interrupt processing to HAL
extern "C" void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(gpsUart.getUartHandle());
}

// HAL calls this callback when it receives a char from UART. Forward it to the class
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uartHandle)
{
	gpsUart.charReceivedCB();
}



void initGPS()
{
	gpsUart.init();
}

void vGPSTask(void *pvParameters)
{
	for (;;)
	{
		gpsUart.waitForString();

		while(gpsUart.available())
		{
			int c = gpsUart.readChar();
			SerialUSB.write(c);
			gpsParser.handle(c);
		}
		
		if(gpsParser.available())
		{
			GPSDataModel::instance().processNewGPSFix(gpsParser.read());
			GPSDataModel::instance().processNewSatellitesData(gpsParser.satellites, gpsParser.sat_count);
		}
			
		vTaskDelay(10);
	}
}
