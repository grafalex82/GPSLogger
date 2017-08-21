#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_usart.h>
#include <stm32f1xx_hal_rcc.h>

#include <string.h> // strlen
#include <stdarg.h> // VA

#include "SerialDebugLogger.h"
#include "PrintUtils.h"



void initDebugSerial()
{
	// Enable clocking of corresponding periperhal
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	// Init pins in alternate function mode
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE); //TX pin
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT); //RX pin

	// Prepare for initialization
	LL_USART_Disable(USART2);

	// Init
	LL_USART_SetBaudRate(USART2, HAL_RCC_GetPCLK1Freq(), 115200);
	LL_USART_SetDataWidth(USART2, LL_USART_DATAWIDTH_8B);
	LL_USART_SetStopBitsLength(USART2, LL_USART_STOPBITS_1);
	LL_USART_SetParity(USART2, LL_USART_PARITY_NONE);
	LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
	LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);

	// Finally enable the peripheral
	LL_USART_Enable(USART2);

}

struct SerialTarget: public PrintTarget
{
	virtual void operator()(char c)
	{
		if(c != '\0')
		{
			while (!LL_USART_IsActiveFlag_TXE(USART2))
				;

			LL_USART_TransmitData8(USART2, c);
		}
	}
};

void serialDebugWrite(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SerialTarget target;
	print(target, fmt, args);
	va_end(args);
}

void serialDebugWrite(char c)
{
	SerialTarget target;
	target(c);
}

