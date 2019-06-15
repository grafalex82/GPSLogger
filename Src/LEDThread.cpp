#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_ll_gpio.h>

#include "LEDThread.h"
#include <Arduino_FreeRTOS.h>
#include "USBDebugLogger.h"
#include "SerialDebugLogger.h"

static volatile uint8_t ledStatus = 0xff;

// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
	const uint32_t ledG = LL_GPIO_PIN_14;
	const uint32_t ledR = LL_GPIO_PIN_15;
	bool inited = false;
public:
	LEDDriver()
	{

	}

	void init()
	{
		if(inited)
			return;

		//enable clock to the GPIOB peripheral
		__HAL_RCC_GPIOB_CLK_ENABLE();

		// Init LED pins as output
		LL_GPIO_SetPinMode(GPIOB, ledG, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(GPIOB, ledG, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(GPIOB, ledG, LL_GPIO_SPEED_FREQ_LOW);
		LL_GPIO_SetOutputPin(GPIOB, ledG);
		LL_GPIO_SetPinMode(GPIOB, ledR, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(GPIOB, ledR, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(GPIOB, ledR, LL_GPIO_SPEED_FREQ_LOW);
		LL_GPIO_SetOutputPin(GPIOB, ledR);

		inited = true;
	}

	void turnOnG()
	{
		LL_GPIO_ResetOutputPin(GPIOB, ledG);
	}

	void turnOffG()
	{
		LL_GPIO_SetOutputPin(GPIOB, ledG);
	}

	void toggleG()
	{
		LL_GPIO_TogglePin(GPIOB, ledG);
	}

	void turnOnR()
	{
		LL_GPIO_ResetOutputPin(GPIOB, ledR);
	}

	void turnOffR()
	{
		LL_GPIO_SetOutputPin(GPIOB, ledR);
	}

	void toggleR()
	{
		LL_GPIO_TogglePin(GPIOB, ledR);
	}
};

static LEDDriver led;

void blink(uint8_t status)
{
	led.init();

	for(int i=0; i<3; i++)
	{
		led.turnOnR();
		if(status & 0x4)
			HAL_Delay(300);
		else
			HAL_Delay(100);
		led.turnOffR();

		status <<= 1;

		HAL_Delay(200);
	}
}

void setLedStatus(uint8_t status)
{
	ledStatus = status;
}

__attribute__((noreturn))
void halt(uint8_t status)
{
	led.init();

	while(true)
	{
		blink(status);

		HAL_Delay(700);
	}
}

__attribute__((noreturn))
void vLEDThread(void *)
{
	led.init();

	// Just blink once in 2 seconds
	for (;;)
	{
		vTaskDelay(2000);

		if(ledStatus == 0xff)
		{
			led.turnOnG();
			vTaskDelay(100);
			led.turnOffG();
		}
		else
		{
			blink(ledStatus);
		}

		usbDebugWrite("Test\n");
		//serialDebugWrite("SerialTest\n");
	}
}
