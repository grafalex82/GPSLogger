#ifndef LEDDRIVER_H
#define LEDDRIVER_H

// Class to encapsulate working with onboard LED(s)
//
// Note: this class initializes corresponding pins in the constructor.
//       May not be working properly if objects of this class are created as global variables
class LEDDriver
{
public:
	LEDDriver();

	void turnOn();
	void turnOff();
	void toggle();
};

#endif // LEDDRIVER_H
