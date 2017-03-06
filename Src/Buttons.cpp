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

// ButtonIDs
enum ButtonID
{
	NO_BUTTON,
	SEL_BUTTON,
	OK_BUTTON
};

// Pin reading function
inline bool getButtonState(uint8 pin)
{
	return !digitalRead(pin); // Inverted logic due to using pullups instead of pulldown
}

/// Return ID of the pressed button (perform debounce first)
ButtonID getCurPressedButton() 
{
	if(getButtonState(SEL_BUTTON_PIN))
	{
		// dobouncing
		vTaskDelay(DEBOUNCE_DURATION);
		if(getButtonState(SEL_BUTTON_PIN))
			return SEL_BUTTON;
	}

	if(getButtonState(OK_BUTTON_PIN))
	{
		// dobouncing
		vTaskDelay(DEBOUNCE_DURATION);
		if(getButtonState(OK_BUTTON_PIN))
		return OK_BUTTON;
	}

	return NO_BUTTON;
}

// Initialize buttons related stuff
void initButtons()
{
	// Set up button pins
	pinMode(SEL_BUTTON_PIN, INPUT_PULLUP); // TODO: using PullUps is an AVR legacy. Consider changing this to pull down
	pinMode(OK_BUTTON_PIN, INPUT_PULLUP);  //  so pin state match human logic expectations
}

// Buttons polling thread function
void vButtonsTask(void *pvParameters)
{
	for (;;)
	{
		// Wait for a button
		if (getCurPressedButton() != NO_BUTTON)
		{
			// Button pressed. Waiting for release
			TickType_t startTime = xTaskGetTickCount();
			while(getCurPressedButton() != NO_BUTTON)
				vTaskDelay(ACTIVE_POLL_PERIOD);
				
			// calc duration
			TickType_t duration = xTaskGetTickCount() - startTime;
			
			if(duration > VERY_LONG_PRESS_DURATION)
				Serial.println("Very Long press detected");
			else
			if(duration > LONG_PRESS_DURATION)
				Serial.println("Long press detected");
			else
				Serial.println("Short click");
		}
		
		// TODO: Use different polling periods depending on global system state (off/idle/active)
		vTaskDelay(ACTIVE_POLL_PERIOD);
	}
}