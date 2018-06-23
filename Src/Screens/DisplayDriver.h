#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include <Adafruit_SSD1306.h>

#define SSD1306_I2C_ADDRESS   0x3C << 1  // account LSB for R/W flag

class DisplayDriver : public ISSD1306Driver
{
	// address of the display on I2C bus
	static const int8_t i2c_addr = SSD1306_I2C_ADDRESS;

public:
	DisplayDriver();

	virtual void begin();
	virtual void sendCommand(uint8_t cmd);
	virtual void sendData(uint8_t * data, size_t size);
};

#endif // DISPLAYDRIVER_H
