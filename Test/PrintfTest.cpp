#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

char outBuf[256];
uint16_t curPos = 0;

void usbDebugWriteInternal(const char *buffer, size_t size, bool reverse = false)
{
	while(size > 0)
	{
		if(reverse)
			buffer--;

		outBuf[curPos++] = *buffer;

		if(!reverse)
			buffer++;

		size--;
	}
}

void printBuffer()
{
	outBuf[curPos] = 0;
	printf("%s\n", outBuf);
}

void verifyBuffer(const char * expectedStr)
{
	outBuf[curPos] = 0;
	if(!strcmp(expectedStr, outBuf))
		printf("OK:     \"%s\"\n", expectedStr);
	else		
		printf("FAILED: expected \"%s\" but got \"%s\"\n", expectedStr, outBuf);
	
	// Prepare for the next test
	curPos = 0;	
}

////////////////////////////////////////////////////////
// Paste code from USBDebugLogger.cpp here
////////////////////////////////////////////////////////

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
size_t PrintNum(unsigned int value, uint8_t radix, char * buf, uint8_t width, char padSymbol)
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

void usbDebugWrite(const char * fmt, ...)
{
	va_list v;
	va_start(v, fmt);

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
			usbDebugWriteInternal(chunkStart, chunkSize);

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
				size_t len = PrintNum(va_arg(v, int), 10, buf, width, padSymbol);
				usbDebugWriteInternal(buf + len, len, true);
				break;
			}
			case 'x':
			case 'X':
			{
				char buf[9];
				size_t len = PrintNum(va_arg(v, int), 16, buf, width, padSymbol);
				usbDebugWriteInternal(buf + len, len, true);
				break;
			}
			case 's':
			{
				char * str = va_arg(v, char*);
				usbDebugWriteInternal(str, strlen(str));
				break;
			}
			case '%':
			{
				usbDebugWriteInternal(fmt-1, 1);
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
		usbDebugWriteInternal(chunkStart, chunkSize - 1); // Not including terminating NULL

	va_end(v);
}

////////////////////////////////////////////////////////
// End of tested code
////////////////////////////////////////////////////////


int main()
{
	printf("Running tests\n");

	usbDebugWrite("A test string");
	verifyBuffer("A test string");

	usbDebugWrite("An string '%s' value", "Test String");
	verifyBuffer("An string 'Test String' value");

	usbDebugWrite("An string '%s' value", "");
	verifyBuffer("An string '' value");

	usbDebugWrite("A percent '%%' symbol");
	verifyBuffer("A percent '%' symbol");

	usbDebugWrite("An integer '%d' value", 0);
	verifyBuffer("An integer '0' value");

	usbDebugWrite("An integer '%d' value", 5);
	verifyBuffer("An integer '5' value");

	usbDebugWrite("An integer '%d' value", 12345678);
	verifyBuffer("An integer '12345678' value");

	usbDebugWrite("An integer '%4d' with width", 0);
	verifyBuffer("An integer '   0' with width");

	usbDebugWrite("An integer '%04d' with width", 0);
	verifyBuffer("An integer '0000' with width");

	usbDebugWrite("An integer '%4d' with width", 7);
	verifyBuffer("An integer '   7' with width");

	usbDebugWrite("An integer '%04d' with width", 8);
	verifyBuffer("An integer '0008' with width");

	usbDebugWrite("An integer '%4d' with width", 123);
	verifyBuffer("An integer ' 123' with width");

	usbDebugWrite("An integer '%04d' with width", 456);
	verifyBuffer("An integer '0456' with width");

	usbDebugWrite("An integer '%4d' with width", 4321);
	verifyBuffer("An integer '4321' with width");

	usbDebugWrite("An integer '%04d' with width", 8765);
	verifyBuffer("An integer '8765' with width");

	usbDebugWrite("An integer '%4d' with width", 123456); //Value does not fit to 4 digits, but will be printed as 6 digit one
	verifyBuffer("An integer '123456' with width");

	usbDebugWrite("A hex '%x' value", 0);
	verifyBuffer("A hex '0' value");

	usbDebugWrite("A hex '%x' value", 0xA);
	verifyBuffer("A hex 'A' value");

	usbDebugWrite("A hex '%x' value", 0xABCDEF01);
	verifyBuffer("A hex 'ABCDEF01' value");

	usbDebugWrite("A hex '%03x' value", 0xc);
	verifyBuffer("A hex '00C' value");

	usbDebugWrite("A hex '%3x' value", 0xd);
	verifyBuffer("A hex '  D' value");

	usbDebugWrite("A hex '%03x' value", 0xDEF);
	verifyBuffer("A hex 'DEF' value");

	usbDebugWrite("A hex '%03x' value", 0xABCD);  //Value does not fit to 3 digits, but will be printed as 4 digit one
	verifyBuffer("A hex 'ABCD' value");

	usbDebugWrite("Multiple '%d' Integer '%d' values", 3, 5);
	verifyBuffer("Multiple '3' Integer '5' values");


	printf("\nTests Finished\n");
}
