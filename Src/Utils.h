#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h> // for uint16_t
#include <Printable.h>

// Forward declarations
typedef uint32_t clock_t;
namespace NeoGPS
{
	struct time_t;	
}

void printNumber(char * buf, uint16_t value, uint8_t digits, bool leadingZeros = true);

class TimePrinter : public Printable
{
	char buf[9]; // 6 digits, 2 x ':' and terminating 0
	
public:
	//TimePrinter(clock_t time);
	TimePrinter(const NeoGPS::time_t & time);
	//TimePrinter(uint8 h, uint8 m, uint8 s);
	
	virtual size_t printTo(Print& p) const;
};

#endif //__UTILS_H__
