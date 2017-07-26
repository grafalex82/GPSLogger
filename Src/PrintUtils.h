#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h> // for uint16_t
#include <stdarg.h>
#include <Printable.h>

// Forward declarations
typedef uint32_t clock_t;
namespace NeoGPS
{
	struct time_t;	
}

// Base class for char consumer functor
struct PrintTarget
{
	virtual void operator()(char c) = 0;
	virtual void operator()(const char *buffer, size_t size, bool reverse = false)
	{
		// Copy data to the buffer
		for(size_t i=0; i < size; i++)
		{
			if(reverse)
				--buffer;

			operator()(*buffer);

			if(!reverse)
				buffer++;
		}
	}
};

// Print formatted string to a passed char consumer
void print(PrintTarget & target, const char * fmt, va_list args);
void print(PrintTarget & target, const char * fmt, ...);

// Print a formatted string to a buffer
void bufprint(char * buf, size_t n, const char * fmt, ...);

// Print a formatted string to display at current position
void printToDisplay(const char * fmt, ...);



/// Helper function to print a single integer with specified width to a passed buffer
void printNumber(char * buf, uint16_t value, uint8_t digits, bool leadingZeros = true);


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
