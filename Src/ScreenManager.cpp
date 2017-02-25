#include <stddef.h> //for NULL

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

#include "ScreenManager.h"

#include "CurrentPositionScreen.h"
#include "CurrentTimeScreen.h"

Adafruit_SSD1306 display;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Pins assignment
const uint8_t SEL_BUTTON_PIN = 2;
const uint8_t OK_BUTTON_PIN = 3;

// Timeouts
const unsigned long DEBOUNCE_TIMEOUT = 30;
const unsigned long MESSAGE_BOX_DIRATION = 5000;

// Stack of nested screens
Screen * screenStack[3];
int screenIdx = 0;

enum State
{
	IDLE_DISPLAY_OFF,
	IDLE,
	MESSAGE_BOX,
	BUTTON_PRESSED,
};

State state = IDLE; // Current state
unsigned long lastEventMillis = 0; // Milis() for last state change

enum ButtonsEnum
{
	NO_BUTTON,
	SEL_BUTTON,
	OK_BUTTON
};

ButtonsEnum curPressedButton = NO_BUTTON;

int8_t getCurPressedButton()
{
	if(!digitalRead(SEL_BUTTON_PIN))
	return SEL_BUTTON;

	if(!digitalRead(OK_BUTTON_PIN))
	return OK_BUTTON;
	
	return NO_BUTTON;
}

void setCurrentScreen(Screen * screen)
{
	screenStack[screenIdx] = screen;
}

Screen * getCurrentScreen()
{
	return screenStack[screenIdx];
}

void enterChildScreen(Screen * screen)
{
	screenIdx++; //TODO limit this
	screenStack[screenIdx] = screen;
}

void backToParentScreen()
{
	if(screenIdx)
		screenIdx--;
}

void initDisplay()
{
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.setTextColor(WHITE);				// Assuming all subsequent commands draw in white color

	// We will be setting up buttons as well here	
	pinMode(SEL_BUTTON_PIN, INPUT_PULLUP);
	pinMode(OK_BUTTON_PIN, INPUT_PULLUP);
}

void initScreens()
{
	Screen * screen = createCurrentTimeScreen();
	setCurrentScreen(screen);
	screen = screen->addScreen(new CurrentPositionScreen());
}

// Display information according to current state and screen
void drawDisplay()
{
	switch(state)
	{
		// Just update display if idle
		case IDLE:
		{
			display.clearDisplay();
			Screen * currentScreen = getCurrentScreen();
			currentScreen->drawHeader();
			currentScreen->drawScreen();
			display.display();
			break;
		}
		
		// Draw a message box if requested
		case MESSAGE_BOX:
		{
			display.clearDisplay();
			display.setFont(NULL);
			display.drawRect(2, 2, 126, 30, 1);
			//display.drawRoundRect(2, 2, 126, 30, 4, 1);
			display.setCursor(10, 12);
			display.print(F("Message Box!"));
			display.display();
			break;
		}

		// Nothing to do for other states
		default:
		break;
	}
}

void processState()
{
	switch(state)
	{
		// Check if user pressed a button
		case IDLE:
		// if so, try to debounce it first
		if (!curPressedButton && (curPressedButton = getCurPressedButton()))  // Button was not pressed before and now gets pressed
		{
			lastEventMillis = millis();
			state = BUTTON_PRESSED;
		}
		
		break;
		
		// Process button release
		case BUTTON_PRESSED:
		{
			ButtonsEnum prevButtonState = curPressedButton;
			curPressedButton = getCurPressedButton();
			
			// Check if button was released after debouncing timeout
			if (prevButtonState != NO_BUTTON && curPressedButton == NO_BUTTON && (millis() - lastEventMillis > DEBOUNCE_TIMEOUT))
			{
				// TODO: Process long press here

				// process button according to current screen
				Screen * currentScreen = getCurrentScreen();
				if(prevButtonState == SEL_BUTTON)
				currentScreen->onSelButton();
				
				if(prevButtonState == OK_BUTTON)
				{
					//currentScreen->onOkButton();
					
					// TODO: Create a function for message boxes
					state = MESSAGE_BOX;
					lastEventMillis = millis();
					break;
				}

				// Buttons processed - returning to IDLE
				state = IDLE;
			}

			break;
		}
		
		// Displaying message box for desired duration
		case MESSAGE_BOX:
		if (millis() - lastEventMillis > MESSAGE_BOX_DIRATION)
		state = IDLE;
		break;
		
		default:
		break;
	}
}