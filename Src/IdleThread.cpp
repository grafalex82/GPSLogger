#include <Arduino_FreeRTOS.h>

#include "IdleThread.h"

static const uint8_t periodLen = 9; // 2^periodLen ticks

volatile TickType_t curIdleTicks = 0;
volatile TickType_t lastCountedTick = 0;
volatile TickType_t lastCountedPeriod = 0;
volatile TickType_t lastPeriodIdleValue = 0;
volatile TickType_t minIdleValue = 1 << periodLen;

extern "C" void vApplicationIdleHook( void )
{
	// Process idle tick counter
	volatile TickType_t curTick = xTaskGetTickCount();
	if(curTick != lastCountedTick)
	{
		curIdleTicks++;
		lastCountedTick = curTick;
	}
	
	// Store idle metrics each ~0.5 seconds (512 ticks)
	curTick >>= periodLen;
	if(curTick >  lastCountedPeriod)
	{
		lastPeriodIdleValue = curIdleTicks;
		curIdleTicks = 0;
		lastCountedPeriod = curTick;
		
		// Store the max value
		if(lastPeriodIdleValue < minIdleValue)
			minIdleValue = lastPeriodIdleValue;
	}
}

float getCPULoad()
{
	return 100. - 100. * lastPeriodIdleValue /  (1 << periodLen);
}

float getMaxCPULoad()
{
	return 100. - 100. * minIdleValue /  (1 << periodLen);
}

extern "C" void yield()
{
	//taskYIELD();
}
