#ifndef SDTHREAD_H
#define SDTHREAD_H

#include <stdint.h>

// Maximum lenth of a single raw GPS data message (not including terminating \n)
const uint8_t maxRawGPSDataLen = 80;
// Returns a pointer to an internal buffer where GPS thread can accumulate raw GPS data (not more than maxRawGPSDataLen symbols)
char * requestRawGPSBuffer();
// Confirm that internal GPS buffer contains len chars and store them to raw GPS dump file
void ackRawGPSData(uint8_t len);

bool cardRead(uint32_t lba, uint8_t * pBuf, uint32_t blocksCount);

// A thread that owns communication with SD card
void initSDThread();
void vSDThread(void *pvParameters);
void xSDTestThread(void *pvParameters);


#endif // SDTHREAD_H
