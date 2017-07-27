#ifndef LEDDRIVER_H
#define LEDDRIVER_H

// A thread that is responsible for showing current device status via onboard LED(s)
void vLEDThread(void *pvParameters);

void blink(uint8_t status);
void setLedStatus(uint8_t status);
void halt(uint8_t status);

#endif // LEDDRIVER_H
