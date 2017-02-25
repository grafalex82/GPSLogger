#ifndef __SCREENMANAGER_H__
#define __SCREENMANAGER_H__

class Screen;

void setCurrentScreen(Screen * screen);
Screen * getCurrentScreen();

void enterChildScreen(Screen * screen);
void backToParentScreen();

void initDisplay();
void initScreens();
void drawDisplay();
void processState();

void messageBox(PROGMEM const char * text);

#endif //__SCREENMANAGER_H__
