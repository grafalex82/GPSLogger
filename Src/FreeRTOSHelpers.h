#ifndef _FREERTOSHELPERS_H_
#define _FREERTOSHELPERS_H_

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


#endif //_FREERTOSHELPERS_H_