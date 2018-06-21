#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include <stdint.h>

// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
	static const uint32_t pin;

	bool inited = false;
public:
	LEDDriver() {}
	void init();

	void turnOn();
	void turnOff();
	void toggle();
};

#endif // LEDDRIVER_H
