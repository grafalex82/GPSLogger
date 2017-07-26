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

/// Helper function to print a single integer with specified width to a passed buffer
void printNumber(char * buf, uint16_t value, uint8_t digits, bool leadingZeros = true);


/// Helper class to print time values as HH:MM:SS
class TimePrinter : public Printable
{
	char buf[9]; // 6 digits, 2 x ':' and terminating 0
	
public:
	//TimePrinter(clock_t time);
	TimePrinter(const NeoGPS::time_t & time);
	//TimePrinter(uint8_t h, uint8_t m, uint8_t s);
	
	virtual size_t printTo(Print& p) const;
};


/// Helper class to print float numbers according to specified options
class FloatPrinter : public Printable
{
	char buf[8]; // Print numbers no longer than 7 digits including sign and point symbols
	uint8_t pos; // position in the buffer with the first meaningful char
	
public:
	FloatPrinter(float value, uint8_t width, bool leadingZeros = false, bool alwaysPrintSign = false);
	
	virtual size_t printTo(Print& p) const;
};



#endif //__UTILS_H__
