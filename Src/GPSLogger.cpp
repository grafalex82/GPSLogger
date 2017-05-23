#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_rcc_ex.h>
#include <stm32f1xx_hal_flash.h>
#include <stm32f1xx_hal_cortex.h>

//#include <MapleFreeRTOS821.h>

//#include "Screens/ScreenManager.h"
//#include "Buttons.h"
//#include "GPSThread.h"

//#include "Arduino.h"

//extern "C" void _init(void)  {}
//int __errno = 0;
  

/*
void vLEDFlashTask(void *pvParameters) 
{
	for (;;) 
	{
		vTaskDelay(2000);
		digitalWrite(PC13, LOW);
		vTaskDelay(100);
		digitalWrite(PC13, HIGH);
	}
}
*/

/*
// the setup function runs once when you press reset or power the board
void setup()
{

	pinMode(PC13, OUTPUT);
	
	SerialUSB.begin(115200);

	initDisplay();
	initButtons();
	initScreens();
	initGPS();
	
	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vLEDFlashTask, "LED Task",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsTask, "Buttons Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

}

// Arduino boilerplate function. Do not write any code here, it would not execute.
void loop()
{
}
*/

// Libc requires _init() to perform some initialization before global constructors run.
// I would love if this symbol is defined as weak in newlib-nano (libc), but it is not.
extern "C" void _init(void)  {}

void SystemClock_Config(void)
{	
	// Set up external oscillator to 72 MHz
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Set up periperal clocking
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	// Set up USB clock
	RCC_PeriphCLKInitTypeDef PeriphClkInit;
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB; 
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5; 
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	// Set up SysTTick to 1 ms
	// TODO: Do we really need this? SysTick is initialized multiple times in HAL
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

int main(void)
{
	// Initialize board and HAL
        HAL_Init();
	SystemClock_Config();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	return 0;
}
