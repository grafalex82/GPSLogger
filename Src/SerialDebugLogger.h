#ifndef SERIALDEBUGLOGGER_H
#define SERIALDEBUGLOGGER_H

#include <stdint.h>
#include <stdlib.h>

void initDebugSerial();
void serialDebugWrite(char c);
void serialDebugWrite(const char * fmt, ...);


#endif // SERIALDEBUGLOGGER_H
