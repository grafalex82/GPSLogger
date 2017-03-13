#include "Utils.h"

// TODO: port this to print to Stream one day
void printNumber(char * buf, uint16_t value, uint8_t digits)
{
	// TODO Fix a case whenvalue is larger than digits specified
	
	while(digits)
	{
		digits--;
		buf[digits] = value % 10 + '0';
		value = value / 10;
	}
}