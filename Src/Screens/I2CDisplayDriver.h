#ifndef I2CDISPLAYDRIVER_H
#define I2CDISPLAYDRIVER_H

#include <Arduino_FreeRTOS.h>
#include <Adafruit_SSD1306.h>

#define SSD1306_I2C_ADDRESS   0x3C

class I2CDisplayDriver : public ISSD1306Driver
{
	// address of the display on I2C bus
	int8_t i2c_addr;

	// I2C device handlers
	I2C_HandleTypeDef handle;
	DMA_HandleTypeDef hdma_tx;

	// transfer synchronization object
	TaskHandle_t xDisplayThread;

public:
	I2CDisplayDriver(int8_t i2caddr = SSD1306_I2C_ADDRESS);

	virtual void begin();
	virtual void sendCommand(uint8_t cmd);
	virtual void sendData(uint8_t * data, size_t size);

	DMA_HandleTypeDef * getDMAHandle();
	void transferCompletedCB();
};

#endif // I2CDISPLAYDRIVER_H
