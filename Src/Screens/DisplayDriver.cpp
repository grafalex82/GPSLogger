#include "TargetPlatform/I2CDriver.h"
#include "DisplayDriver.h"

// A display driver instance
DisplayDriver displayDriver;

DisplayDriver::DisplayDriver()
{
	xDisplayThread = NULL;
}

void DisplayDriver::begin()
{
	// Init sync object
	xDisplayThread = xTaskGetCurrentTaskHandle();
}

void DisplayDriver::sendCommand(uint8_t cmd)
{
	i2cDriver.writeMem(i2c_addr, 0x00, &cmd, 1);
}

void DisplayDriver::sendData(uint8_t * data, size_t size)
{
	// Start data transfer
	i2cDriver.writeMem(i2c_addr, 0x40, data, size);

	// Wait until transfer is completed
	//ulTaskNotifyTake(pdTRUE, 100);
}

void DisplayDriver::transferCompletedCB()
{
	// Resume display thread
	//vTaskNotifyGiveFromISR(xDisplayThread, NULL);
}
