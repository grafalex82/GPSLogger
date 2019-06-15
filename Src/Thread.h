#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <Arduino_FreeRTOS.h>

template<const uint32_t ulStackDepth>
class Thread
{
protected:
	StaticTask_t xTaskBuffer;
	StackType_t xStack[ ulStackDepth ];

public:
	Thread(TaskFunction_t pxTaskCode,
		   const char * const pcName,
		   void * const pvParameters,
		   UBaseType_t uxPriority)
	{
		xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, xStack, &xTaskBuffer);
	}
};

#endif // THREAD_H
