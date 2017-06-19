#include <stddef.h> //for NULL

#include <Arduino_FreeRTOS.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "8x12Font.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "ButtonsThread.h"

#include "DisplayDriver.h"
#include "CurrentPositionScreen.h"
#include "CurrentTimeScreen.h"
#include "SpeedScreen.h"
#include "SatellitesScreen.h"
#include "OdometerScreen.h"
#include "SettingsGroupScreen.h"

extern DisplayDriver displayDriver;
Adafruit_SSD1306 display(&displayDriver, -1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Timeouts
const uint16_t DISPLAY_CYCLE = 100 / portTICK_PERIOD_MS;
const uint16_t MESSAGE_BOX_DURATION = 1000 / portTICK_PERIOD_MS;

// Stack of nested screens
Screen * screenStack[5];
int screenIdx = 0;


// Statically allocated screens
CurrentTimeScreen timeScreen;
CurrentPositionScreen positionScreen;
SpeedScreen speedScreen;
OdometerScreen odometerScreen(0);
SatellitesScreen satellitesScreen;
SettingsGroupScreen rootSettingsScreen;

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
	display.begin(SSD1306_SWITCHCAPVCC, false);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.setTextColor(WHITE);				// Assuming all subsequent commands draw in white color
}

void initScreens()
{
	setCurrentScreen(&timeScreen);
	timeScreen.addScreen(&positionScreen);
	positionScreen.addScreen(&speedScreen);
	speedScreen.addScreen(&odometerScreen);
	odometerScreen.addScreen(&satellitesScreen);
	satellitesScreen.addScreen(&rootSettingsScreen);
}

// Display information according to current state and screen
void drawDisplay()
{
	display.clearDisplay();
	Screen * currentScreen = getCurrentScreen();
	currentScreen->drawHeader();
	currentScreen->drawScreen();
	display.display();
}

void showMessageBox(const char * text)
{
	//Center text
	uint8_t x = 128/2 - strlen_P(text)*8/2 + 1;
			
	// Draw the message
	display.clearDisplay();
	display.setFont(&Monospace8x12Font);
	display.drawRect(1, 1, 126, 30, 1);
	display.setCursor(x, 22);
	display.print(text);
	display.display();
	
	// Wait required duration
	vTaskDelay(MESSAGE_BOX_DURATION);
}

void processButton(const ButtonMessage &msg)
{
	if(msg.button == SEL_BUTTON && msg.event == BUTTON_CLICK)
		getCurrentScreen()->onSelButton();
		
	if(msg.button == OK_BUTTON && msg.event == BUTTON_CLICK)
		getCurrentScreen()->onOkButton();

	// TODO: handle long and very long button presses here
}

void vDisplayTask(void *pvParameters) 
{
	initDisplay();
	initScreens();

	TickType_t lastActionTicks = xTaskGetTickCount();
	
	for (;;)
	{
		// Poll the buttons queue for an event. Process button if pressed, or show current screen as usual if no button pressed
		ButtonMessage msg;
		if(waitForButtonMessage(&msg, DISPLAY_CYCLE))
		{
			processButton(msg);
			
			// Reset display off timer
			lastActionTicks = xTaskGetTickCount();
		}
		
		// Enter display off mode if needed
		if(xTaskGetTickCount() - lastActionTicks > 10000) // TODO Store display off duration in settings
		{
			// Turn off the display
			display.ssd1306_command(SSD1306_DISPLAYOFF);
			
			// Wait for a button
			waitForButtonMessage(&msg, portMAX_DELAY);
			
			// Resume
			display.ssd1306_command(SSD1306_DISPLAYON);
			lastActionTicks = xTaskGetTickCount();
		}
		
		// Draw the current page
		drawDisplay();
	}
}
