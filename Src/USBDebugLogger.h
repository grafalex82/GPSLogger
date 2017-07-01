#ifndef USBDEBUGLOGGER_H
#define USBDEBUGLOGGER_H

#include <stdint.h>
#include <stdlib.h>

void initUsbDebugLogger();
void usbDebugWrite(char c);
void usbDebugWrite(const char * fmt, ...);

#endif // USBDEBUGLOGGER_H
