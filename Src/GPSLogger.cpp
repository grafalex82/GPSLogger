
// FreeRTOS
#include <FreeRTOS.h>
#include "task.h"

//#include "Screens/ScreenManager.h"
//#include "GPSThread.h"

#include "BoardInit.h"
#include "LEDThread.h"
#include "ButtonsThread.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"



extern "C" void _Error_Handler(char * file, int line)
{

}

USBD_HandleTypeDef hUsbDeviceFS;

void reenumerate()
{

	// Initialize PA12 pin
	GPIO_InitTypeDef pinInit;
	pinInit.Pin = GPIO_PIN_12;
	pinInit.Mode = GPIO_MODE_OUTPUT_PP;
	pinInit.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &pinInit);

	// Let host know to enumerate USB devices on the bus
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	for(unsigned int i=0; i<512; i++) {};

	// Restore pin mode
	pinInit.Mode = GPIO_MODE_INPUT;
	pinInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pinInit);
	for(unsigned int i=0; i<512; i++) {};
}

void initUSB()
{
	reenumerate();

	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);

}

extern "C" void USBSerial_Rx_Handler(uint8_t *data, uint16_t len)
{
	CDC_Transmit_FS(data, len);
}


int main(void)
{
	InitBoard();

	//SerialUSB.begin(115200);

	//initDisplay();
	initButtons();
	//initScreens();
	//initGPS();

	initUSB();


	// Set up threads
	// TODO: Consider encapsulating init and task functions into a class(es)
	xTaskCreate(vLEDThread, "LED Thread",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vDisplayTask, "Display Task", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vButtonsThread, "Buttons Thread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(vGPSTask, "GPS Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);

	// Run scheduler and all the threads
	vTaskStartScheduler();

	// Never going to be here
	return 0;
}
