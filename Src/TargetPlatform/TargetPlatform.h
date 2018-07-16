#ifndef TARGET_H
#define TARGET_H

#ifdef STM32F1
	#include <stm32f1xx_hal.h>
	#include <stm32f1xx_ll_gpio.h>
#elif STM32F4
	#include <stm32f4xx_hal.h>
	#include <stm32f4xx_ll_gpio.h>
#endif

void InitBoard();

enum TargetBoard
{
	MCU_STM32F103CB,
	MCU_STM32F407VE
};

static const TargetBoard TARGET_MCU = MCU_STM32F407VE;

template<TargetBoard>
struct ProcessorTraits;

template<>
struct ProcessorTraits<MCU_STM32F407VE>
{
	static const uint32_t selButtonPin = LL_GPIO_PIN_4;
	static const uint32_t okButtonPin = LL_GPIO_PIN_3;
	static void enableButtonsClock() { __HAL_RCC_GPIOE_CLK_ENABLE(); }
	static GPIO_TypeDef * buttonsPort() { return GPIOE; }
};

typedef ProcessorTraits<TARGET_MCU> Board;

#endif //TARGET_H
