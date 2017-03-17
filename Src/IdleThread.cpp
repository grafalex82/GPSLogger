#include <MapleFreeRTOS821.h>

#include "IdleThread.h"

volatile TickType_t idleTicks = 0;
volatile TickType_t lastCountedTick = 0;

extern "C" void vApplicationIdleHook( void )
{
	volatile TickType_t curTick = xTaskGetTickCount();
	if(curTick != lastCountedTick)
	{
		idleTicks++;
		lastCountedTick = curTick;
	}
}

uint32_t getIdleTicks()
{
	return idleTicks;
}