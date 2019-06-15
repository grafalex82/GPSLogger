#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "ButtonsThread.h"

#include "USBDebugLogger.h"

#include <stm32f1xx_ll_gpio.h>

// TODO: perhaps it would be reasonable to detect button press via pin change interrupt

// Pins assignment
static GPIO_TypeDef * const	BUTTONS_PORT = GPIOB;
const uint32_t SEL_BUTTON_PIN = LL_GPIO_PIN_12;
const uint32_t OK_BUTTON_PIN = LL_GPIO_PIN_13;

// Timing constants
const uint32_t DEBOUNCE_DURATION = 1 / portTICK_PERIOD_MS;
const uint32_t LONG_PRESS_DURATION = 500 / portTICK_PERIOD_MS;
const uint32_t VERY_LONG_PRESS_DURATION = 1000 / portTICK_PERIOD_MS;
const uint32_t POWER_OFF_POLL_PERIOD = 1000 / portTICK_PERIOD_MS; // Polling very rare when power is off
const uint32_t IDLE_POLL_PERIOD = 100 / portTICK_PERIOD_MS;		// And little more frequent if we are on
const uint32_t ACTIVE_POLL_PERIOD = 10 / portTICK_PERIOD_MS;		// And very often when user actively pressing buttons

QueueHandle_t buttonsQueue;
StaticQueue_t buttonsQueueBuffer;
ButtonMessage buttonsQueueStorage[3]; // 3 clicks more than enough


// Initialize buttons related stuff
void initButtons()
{
	//enable clock to the GPIOB peripheral
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Set up button pins
	LL_GPIO_SetPinMode(BUTTONS_PORT, SEL_BUTTON_PIN, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(BUTTONS_PORT, OK_BUTTON_PIN, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(BUTTONS_PORT, OK_BUTTON_PIN, LL_GPIO_PULL_DOWN);

	// Initialize buttons queue
	buttonsQueue = xQueueCreateStatic(sizeof(buttonsQueueStorage)/sizeof(ButtonMessage),
									  sizeof(ButtonMessage),
									  reinterpret_cast<uint8_t*>(&buttonsQueueStorage[0]),
									  &buttonsQueueBuffer); // 3 clicks more than enough
}


// Reading button state (perform debounce first)
inline bool getButtonState(uint32_t pin)
{
	if(LL_GPIO_IsInputPinSet(BUTTONS_PORT, pin))
	{
		// dobouncing
		vTaskDelay(DEBOUNCE_DURATION);
		if(LL_GPIO_IsInputPinSet(BUTTONS_PORT, pin))
			return true;
	}

	return false;
}


/// Return ID of the pressed button (perform debounce first)
ButtonID getPressedButtonID() 
{
	if(getButtonState(SEL_BUTTON_PIN))
		return SEL_BUTTON;

	if(getButtonState(OK_BUTTON_PIN))
		return OK_BUTTON;

	return NO_BUTTON;
}


// Buttons polling thread function
void vButtonsThread(void *pvParameters)
{
	for (;;)
	{
		// Wait for a button
		ButtonID btn = getPressedButtonID();
		if (btn != NO_BUTTON)
		{
			// Button pressed. Waiting for release
			TickType_t startTime = xTaskGetTickCount();
			while(getPressedButtonID() != NO_BUTTON)
				vTaskDelay(ACTIVE_POLL_PERIOD);

			// Prepare message to send
			ButtonMessage msg;
			msg.button = btn;
				
			// calc duration
			TickType_t duration = xTaskGetTickCount() - startTime;
			if(duration > VERY_LONG_PRESS_DURATION)
				msg.event = BUTTON_VERY_LONG_PRESS;
			else
			if(duration > LONG_PRESS_DURATION)
				msg.event = BUTTON_LONG_PRESS;
			else
				msg.event = BUTTON_CLICK;

			// Send the message
			xQueueSend(buttonsQueue, &msg, 0);
		}
		
		// TODO: Use different polling periods depending on global system state (off/idle/active)
		vTaskDelay(ACTIVE_POLL_PERIOD);
	}
}


bool waitForButtonMessage(ButtonMessage * msg, TickType_t xTicksToWait)
{
	return xQueueReceive(buttonsQueue, msg, xTicksToWait);
}
