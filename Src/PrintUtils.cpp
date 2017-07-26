#include <Print.h>
#include <NeoTime.h>

#include "PrintUtils.h"

// Stores provided chars into the buffer (respecting capacity and terminating zeroes)
struct CharBufConsumer : public CharConsumer
{
	char * m_buf;
	size_t m_capacityLeft;

	CharBufConsumer(char * buf, size_t n)
		: m_buf(buf), m_capacityLeft(n)
	{}

	virtual void operator()(char c)
	{
		// For the last slot we can set only terminating zero (regardless of input)
		if(m_capacityLeft <= 1)
		{
			*m_buf = '\0';
			return;
		}

		*m_buf = c;
		m_buf++;
		m_capacityLeft--;
	}
};

// sprintf implementation takes more than 10kb and adding heap to the project. I think this is
// too much for the functionality I need
//
// Below is a homebrew printf-like dumping function which accepts:
// - %d for digits
// - %x for numbers as HEX
// - %s for strings
// - %% for percent symbol
//
// Implementation supports also value width as well as zero padding

// Print the number to the buffer (in reverse order)
// Returns number of printed symbols
static size_t PrintNum(unsigned int value, uint8_t radix, char * buf, uint8_t width, char padSymbol)
{
	//TODO check negative here

	size_t len = 0;

	// Print the number
	do
	{
		char digit = value % radix;
		*(buf++) = digit < 10 ? '0' + digit : 'A' - 10 + digit;
		value /= radix;
		len++;
	}
	while (value > 0);

	// Add zero padding
	while(len < width)
	{
		*(buf++) = padSymbol;
		len++;
	}

	return len;
}

void print(CharConsumer & consumeChars, const char * fmt, va_list args)
{
	const char * chunkStart = fmt;
	size_t chunkSize = 0;

	char ch;
	do
	{
		// Get the next byte
		ch = *(fmt++);

		// Just copy the regular characters
		if(ch != '%')
		{
			chunkSize++;
			continue;
		}

		// We hit a special symbol. Dump string that we processed so far
		if(chunkSize)
			consumeChars(chunkStart, chunkSize);

		// Process special symbols

		// Check if zero padding requested
		char padSymbol = ' ';
		ch = *(fmt++);
		if(ch == '0')
		{
			padSymbol = '0';
			ch = *(fmt++);
		}

		// Check if width specified
		uint8_t width = 0;
		if(ch > '0' && ch <= '9')
		{
			width = ch - '0';
			ch = *(fmt++);
		}

		// check the format
		switch(ch)
		{
			case 'd':
			case 'u':
			{
				char buf[12];
				size_t len = PrintNum(va_arg(args, int), 10, buf, width, padSymbol);
				consumeChars(buf + len, len, true);
				break;
			}
			case 'x':
			case 'X':
			{
				char buf[9];
				size_t len = PrintNum(va_arg(args, int), 16, buf, width, padSymbol);
				consumeChars(buf + len, len, true);
				break;
			}
			case 's':
			{
				char * str = va_arg(args, char*);
				consumeChars(str, strlen(str));
				break;
			}
			case '%':
			{
				consumeChars(fmt-1, 1);
				break;
			}
			default:
				// Otherwise store it like a regular symbol as a part of next chunk
				fmt--;
				break;
		}

		chunkStart = fmt;
		chunkSize=0;
	}
	while(ch != 0);

	if(chunkSize)
		consumeChars(chunkStart, chunkSize); // Including terminating NULL
}

void cprintf(CharConsumer & consumeChars, const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	print(consumeChars, fmt, args);
	va_end(args);
}

void bufprint(char * buf, size_t n, const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	CharBufConsumer cons(buf, n);
	print(cons, fmt, args);
	va_end(args);
}


// TODO: port this to print to Stream one day
void printNumber(char * buf, uint16_t value, uint8_t digits, bool leadingZeros)
{
	// TODO Fix a case when value is larger than digits specified

	bool drawSpaces = false;	
	while(digits)
	{
		digits--;
		buf[digits] = drawSpaces ? ' ' : value % 10 + '0';
		value = value / 10;

		// We must print at least one symbol, so this condition is here		
		if(!leadingZeros && !value)
			drawSpaces = true;
	}
}


TimePrinter::TimePrinter(const NeoGPS::time_t & time)
{
	printNumber(buf, time.hours, 2);
	buf[2] = ':';
	printNumber(buf+3, time.minutes, 2);
	buf[5] = ':';
	printNumber(buf+6, time.seconds, 2);
	buf[8] = '\0';
}

size_t TimePrinter::printTo(Print& p) const
{
	return p.print(buf);
}

FloatPrinter::FloatPrinter(float value, uint8_t width, bool leadingZeros, bool alwaysPrintSign)
{
    // reserve a space for sign
    uint8_t minpos = 0;
    if(alwaysPrintSign || value < 0)
		minpos++;

    // absolute value to print, deal with sign later
    float v = value;
    if(v < 0)
		v = 0. - v;
    
    // floating point position will depend on the value
    uint8_t precision = 0;
    if(v < 100)
    {
	    v *= 10;
	    precision++;
    }
    if(v < 100) // doing this twice
    {
	    v *= 10;
	    precision++;
    }

    uint32_t iv = v + 0.5; // we will be operating with integers

    // Filling the buffer starting from the right
    pos = width;
    buf[pos] = '\0';
    bool onceMore = true; // Print at least one zero before dot
    while((iv > 0 || onceMore) && (pos > minpos))
    {
	    pos--;
	    onceMore = false;
	    
	    // Fill one digit
	    buf[pos] = iv % 10 + '0';
	    iv /= 10;
	    
	    // Special case for printing point
	    // Trick used: if precision is 0 here it will become 255 and dot will never be printed (assuming the buffer size is less than 255)
	    if(--precision == 0)
	    {
		    buf[--pos] = '.';
		    onceMore = true;
	    }
    }
    
    //Print sign
    if(value < 0)
		buf[--pos] = '-';
    else if (alwaysPrintSign)
	    buf[--pos] = '+';
}

size_t FloatPrinter::printTo(Print& p) const
{
	return p.print(buf+pos);
}
