#ifndef BUTTONS_H_
#define BUTTONS_H_

/// Button event happened
enum ButtonEvent
{
	SEL_BUTTON_CLICK,
	SEL_BUTTON_LONG_PRESS,
	OK_BUTTON_CLICK,
	OK_BUTTON_LONG_PRESS
};

// Initialize buttons, ports and related stuff
void initButtons();

// Buttons are monitored in a dedicated task.
// When button click or long press is detected corresponding message is sent to UI task
void vButtonsTask(void *pvParameters);

#endif /* BUTTONS_H_ */