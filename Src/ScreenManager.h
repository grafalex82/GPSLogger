#ifndef __SCREENMANAGER_H__
#define __SCREENMANAGER_H__

// Forward declarations
class Screen;

/// Set current screen to display
void setCurrentScreen(Screen * screen);
/// Retrieve currently set screen
Screen * getCurrentScreen();

/// Enter into a nested screen (previous screen is saved in screen stack)
void enterChildScreen(Screen * screen);
/// Return to a parent screen stored in stack
void backToParentScreen();

/// Initialize display stuff
void initDisplay();
/// Initialize screens and their relationship
void initScreens();
/// Draw current screen
void drawDisplay();
/// Show a message box
void showMessageBox(const char * text);

/// Display thread. Responsible for displaying an actual screen and handle messages from buttons thread
void vDisplayTask(void *pvParameters);

#endif //__SCREENMANAGER_H__
