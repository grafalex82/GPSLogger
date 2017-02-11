#include <stddef.h> //for NULL
#include <HardwareSerial.h>

#include "ScreenManager.h"

Screen * screenStack[3];
int screenIdx = 0;

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
