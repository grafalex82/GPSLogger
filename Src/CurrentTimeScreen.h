#ifndef _TIME_SCREEN_H_
#define _TIME_SCREEN_H_

#include "ParentScreen.h"

class CurrentTimeScreen: public ParentScreen
{
public:
  virtual void drawScreen() const;
  
  CurrentTimeScreen();
};

#endif //_TIME_SCREEN_H_
