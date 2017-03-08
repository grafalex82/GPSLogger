#include <MapleFreeRTOS821.h>
#include "Buttons.h"

// TODO: perhaps it would be reasonable to detect button press via pin change interrupt

// Pins assignment
const uint8 SEL_BUTTON_PIN = PC14;
const uint8 OK_BUTTON_PIN = PC15;

// Timing constants
const uint32 DEBOUNCE_DURATION = 1 / portTICK_PERIOD_MS;
const uint32 LONG_PRESS_DURATION = 500 / portTICK_PERIOD_MS;
const uint32 VERY_LONG_PRESS_DURATION = 1000 / portTICK_PERIOD_MS;
const uint32 POWER_OFF_POLL_PERIOD = 1000 / portTICK_PERIOD_MS; // Polling very rare when power is off
const uint32 IDLE_POLL_PERIOD = 100 / portTICK_PERIOD_MS;		// And little more frequent if we are on
const uint32 ACTIVE_POLL_PERIOD = 10 / portTICK_PERIOD_MS;		// And very often when user actively pressing buttons

QueueHandle_t buttonsQueue;

// Reading button state (perform debounce first)
inline bool getButtonState(uint8 pin)
{
	if(digitalRead(pin))
	{
		// dobouncing
		vTaskDelay(DEBOUNCE_DURATION);
		if(digitalRead(pin))
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

// Initialize buttons related stuff
void initButtons()
{
	// Set up button pins
	pinMode(SEL_BUTTON_PIN, INPUT_PULLDOWN);
	pinMode(OK_BUTTON_PIN, INPUT_PULLDOWN);
	
	// Initialize buttons queue
	buttonsQueue = xQueueCreate(3, sizeof(ButtonMessage)); // 3 clicks more than enough
}

// Buttons polling thread function
void vButtonsTask(void *pvParameters)
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