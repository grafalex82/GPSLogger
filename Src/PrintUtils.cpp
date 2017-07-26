#include <Print.h>
#include <NeoTime.h>

#include "Utils.h"

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
