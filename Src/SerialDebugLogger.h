#ifndef SERIALDEBUGLOGGER_H
#define SERIALDEBUGLOGGER_H

#include <stdint.h>
#include <stdlib.h>

void initDebugSerial();
void serialDebugWrite(char c);
extern "C" void serialDebugWrite(const char * fmt, ...);
extern "C" void serialDebugWriteC(char c);


#endif // SERIALDEBUGLOGGER_H
