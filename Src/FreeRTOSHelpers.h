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

	operator QueueHandle_t() const
	{
		return queueHandle;
	}
};

#endif //FREERTOSHELPERS_H_
