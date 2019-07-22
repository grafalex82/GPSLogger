#ifndef FREERTOSHELPERS_H_
#define FREERTOSHELPERS_H_

#include "Arduino_FreeRTOS.h"

class MutexLocker
{
public:
	MutexLocker(SemaphoreHandle_t mtx)
	{
		mutex = mtx;
		xSemaphoreTake(mutex, portMAX_DELAY);
	}
	
	~MutexLocker()
	{
		xSemaphoreGive(mutex);
	}

private:
	SemaphoreHandle_t mutex;	
};

template<class T, size_t size>
class Queue
{
	QueueHandle_t	queueHandle;
	StaticQueue_t	queueControlBlock;
	T				queueStorage[size];

public:
	Queue()
	{
		queueHandle = xQueueCreateStatic(size,
										 sizeof(T),
										 reinterpret_cast<uint8_t*>(queueStorage),
										 &queueControlBlock);
	}

	//operator QueueHandle_t() const
	//{
	//	return queueHandle;
	//}

	bool receive(T * val, TickType_t xTicksToWait)
	{
		return xQueueReceive(queueHandle, val, xTicksToWait);
	}

	bool send(T & val, TickType_t xTicksToWait)
	{
		return xQueueSend(queueHandle, &val, xTicksToWait);
	}
};

class Sema
{
	SemaphoreHandle_t sema;
	StaticSemaphore_t semaControlBlock;

public:
	Sema()
	{
		sema = xSemaphoreCreateBinaryStatic(&semaControlBlock);
	}

	BaseType_t giveFromISR()
	{
		BaseType_t xHigherPriorityTaskWoken;
		xSemaphoreGiveFromISR(sema, &xHigherPriorityTaskWoken);
		return xHigherPriorityTaskWoken;
	}

	bool give()
	{
		return xSemaphoreGive(sema) == pdTRUE;
	}

	bool take(TickType_t xTicksToWait)
	{
		return xSemaphoreTake(sema, xTicksToWait) == pdTRUE;
	}
};

#endif //FREERTOSHELPERS_H_
