#ifndef LEDTHREAD_H
#define LEDTHREAD_H

#include <stdint.h>

// A thread that is responsible for showing current device status via onboard LED(s)
void vLEDThread(void *pvParameters);

void blink(uint8_t status);
void setLedStatus(uint8_t status);
void halt(uint8_t status);

#endif // LEDTHREAD_H
