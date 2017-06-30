#ifndef USBDEBUGLOGGER_H
#define USBDEBUGLOGGER_H

#include <stdint.h>
#include <stdlib.h>

void initUsbDebugLogger();
void usbDebugWrite(uint8_t c);
void usbDebugWrite(const char * str);
void usbDebugWrite(const uint8_t *buffer, size_t size);

#endif // USBDEBUGLOGGER_H
