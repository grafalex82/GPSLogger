#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "MapleFreeRTOS821.h"

/// ButtonIDs
enum ButtonID
{
	NO_BUTTON,
	SEL_BUTTON,
	OK_BUTTON
};

/// Button event happened
enum ButtonEvent
{
	BUTTON_CLICK,
	BUTTON_LONG_PRESS,
	BUTTON_VERY_LONG_PRESS
};

/// Message structure that is passed from buttons thread to UI task
struct ButtonMessage
{
	ButtonID button;
	ButtonEvent event;
};

/// A message queue between Buttons and UI threads
extern QueueHandle_t buttonsQueue;

// Initialize buttons, ports, queue and related stuff
void initButtons();

// Buttons are being monitored in a dedicated task.
// When button click or long press is detected corresponding message is sent to UI task
void vButtonsTask(void *pvParameters);

#endif /* BUTTONS_H_ */