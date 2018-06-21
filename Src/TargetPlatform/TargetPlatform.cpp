#ifdef STM32F1
	#include <stm32f1xx_hal.h>
#elif STM32F4
	#include <stm32f4xx_hal.h>
#endif

#include "TargetPlatform.h"

// Libc requires _init() to perform some initialization before global constructors run.
// I would love if this symbol is defined as weak in newlib-nano (libc), but it is not.
extern "C" void _init(void)  {}

// Forward declarations
void SystemClock_Config(void);

void InitBoard()
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Initialize board and HAL
	HAL_Init();
	SystemClock_Config(); // Defer to CPU specific configuration routine

	// Set up SysTTick to 1 ms
	// TODO: Do we really need this? SysTick is initialized multiple times in HAL
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration - setting SysTick as lower priority to satisfy FreeRTOS requirements
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
