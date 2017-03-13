#include <stddef.h> //for NULL

#include <MapleFreeRTOS821.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>

#include "8x12Font.h"
#include "ScreenManager.h"
#include "Buttons.h"

#include "CurrentPositionScreen.h"
#include "CurrentTimeScreen.h"
#include "SatellitesScreen.h"

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Timeouts
const uint16 DISPLAY_CYCLE = 100 / portTICK_PERIOD_MS;
const uint16 MESSAGE_BOX_DIRATION = 1000 / portTICK_PERIOD_MS;

// Stack of nested screens
Screen * screenStack[3];
int screenIdx = 0;


// Statically allocated screens
CurrentTimeScreen timeScreen;
CurrentPositionScreen positionScreen;
SatellitesScreen satellitesScreen;

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
}

void initScreens()
{
	setCurrentScreen(&timeScreen);
	timeScreen.addScreen(&positionScreen);
	positionScreen.addScreen(&satellitesScreen);
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
	uint8_t x = 128/2 - strlen_P(text)*8/2;
			
	// Draw the message
	display.clearDisplay();
	display.setFont(&Monospace8x12Font);
	display.drawRect(2, 2, 126, 30, 1);
	display.setCursor(x, 22);
	display.print(text);
	display.display();
	
	// Wait required duration
	vTaskDelay(MESSAGE_BOX_DIRATION);
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
	for (;;)
	{
		// Poll the buttons queue for an event. Process button if pressed, or show current screen as usual if no button pressed
		ButtonMessage msg;
		if(xQueueReceive(buttonsQueue, &msg, DISPLAY_CYCLE))
			processButton(msg);
		
		// Do what we need for current state
		drawDisplay();
	}
}