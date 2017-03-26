#include <Print.h>
#include <NeoTime.h>

#include "Utils.h"

// TODO: port this to print to Stream one day
void printNumber(char * buf, uint16_t value, uint8_t digits, bool leadingZeros)
{
	// TODO Fix a case whenvalue is larger than digits specified

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
