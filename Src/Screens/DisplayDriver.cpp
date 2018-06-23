#include "TargetPlatform/I2CDriver.h"
#include "DisplayDriver.h"

// A display driver instance
DisplayDriver displayDriver;

DisplayDriver::DisplayDriver()
{
}

void DisplayDriver::begin()
{
}

void DisplayDriver::sendCommand(uint8_t cmd)
{
	i2cDriver.writeMem(i2c_addr, 0x00, &cmd, 1);
}

void DisplayDriver::sendData(uint8_t * data, size_t size)
{
	i2cDriver.writeMemDMA(i2c_addr, 0x40, data, size);
}
