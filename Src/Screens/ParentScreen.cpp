#include <stddef.h>
#include "ParentScreen.h"
#include "ScreenManager.h"

ParentScreen::ParentScreen()
{
	childScreen = NULL;
}

Screen * ParentScreen::addChildScreen(Screen * screen)
{
	childScreen = screen;
	return screen;
}

void ParentScreen::onOkButton()
{
	if(childScreen)
		enterChildScreen(childScreen);
	else
		backToParentScreen();
}