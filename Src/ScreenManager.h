#ifndef __SCREENMANAGER_H__
#define __SCREENMANAGER_H__

class Screen;

void setCurrentScreen(Screen * screen);
Screen * getCurrentScreen();

void enterChildScreen(Screen * screen);
void backToParentScreen();


#endif //__SCREENMANAGER_H__
