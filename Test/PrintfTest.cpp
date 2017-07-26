#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

char outBuf[256];
uint16_t curPos = 0;

//////////////////////////////////////////////////
// Begin of tested code. Copy stuff from PrintUtils.cpp and PrintUtils.h here
//////////////////////////////////////////////////

//////////// PrintUtils.h
// Base class for char consumer functor
struct CharConsumer
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

///////////// PrintUtils.cpp

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
//////////////////////////////////////////////////
// End of tested code
//////////////////////////////////////////////////

void printBuffer()
{
	outBuf[255] = 0;
	printf("%s\n", outBuf);
}

void verifyBuffer(const char * expectedStr)
{
	outBuf[255] = 0;
	if(!strcmp(expectedStr, outBuf))
		printf("OK:     \"%s\"\n", expectedStr);
	else		
		printf("FAILED: expected \"%s\" but got \"%s\"\n", expectedStr, outBuf);
	
	// Prepare for the next test
	curPos = 0;	
}

int main()
{
	printf("Running tests\n");

	bufprint(outBuf, 256, "A test string");
	verifyBuffer("A test string");

	bufprint(outBuf, 256, "An string '%s' value", "Test String");
	verifyBuffer("An string 'Test String' value");

	bufprint(outBuf, 256, "An string '%s' value", "");
	verifyBuffer("An string '' value");

	bufprint(outBuf, 256, "A percent '%%' symbol");
	verifyBuffer("A percent '%' symbol");

	bufprint(outBuf, 256, "An integer '%d' value", 0);
	verifyBuffer("An integer '0' value");

	bufprint(outBuf, 256, "An integer '%d' value", 5);
	verifyBuffer("An integer '5' value");

	bufprint(outBuf, 256, "An integer '%d' value", 12345678);
	verifyBuffer("An integer '12345678' value");

	bufprint(outBuf, 256, "An integer '%4d' with width", 0);
	verifyBuffer("An integer '   0' with width");

	bufprint(outBuf, 256, "An integer '%04d' with width", 0);
	verifyBuffer("An integer '0000' with width");

	bufprint(outBuf, 256, "An integer '%4d' with width", 7);
	verifyBuffer("An integer '   7' with width");

	bufprint(outBuf, 256, "An integer '%04d' with width", 8);
	verifyBuffer("An integer '0008' with width");

	bufprint(outBuf, 256, "An integer '%4d' with width", 123);
	verifyBuffer("An integer ' 123' with width");

	bufprint(outBuf, 256, "An integer '%04d' with width", 456);
	verifyBuffer("An integer '0456' with width");

	bufprint(outBuf, 256, "An integer '%4d' with width", 4321);
	verifyBuffer("An integer '4321' with width");

	bufprint(outBuf, 256, "An integer '%04d' with width", 8765);
	verifyBuffer("An integer '8765' with width");

	bufprint(outBuf, 256, "An integer '%4d' with width", 123456); //Value does not fit to 4 digits, but will be printed as 6 digit one
	verifyBuffer("An integer '123456' with width");

	bufprint(outBuf, 256, "A hex '%x' value", 0);
	verifyBuffer("A hex '0' value");

	bufprint(outBuf, 256, "A hex '%x' value", 0xA);
	verifyBuffer("A hex 'A' value");

	bufprint(outBuf, 256, "A hex '%x' value", 0xABCDEF01);
	verifyBuffer("A hex 'ABCDEF01' value");

	bufprint(outBuf, 256, "A hex '%03x' value", 0xc);
	verifyBuffer("A hex '00C' value");

	bufprint(outBuf, 256, "A hex '%3x' value", 0xd);
	verifyBuffer("A hex '  D' value");

	bufprint(outBuf, 256, "A hex '%03x' value", 0xDEF);
	verifyBuffer("A hex 'DEF' value");

	bufprint(outBuf, 256, "A hex '%03x' value", 0xABCD);  //Value does not fit to 3 digits, but will be printed as 4 digit one
	verifyBuffer("A hex 'ABCD' value");

	bufprint(outBuf, 256, "Multiple '%d' Integer '%d' values", 3, 5);
	verifyBuffer("Multiple '3' Integer '5' values");


	printf("\nTests Finished\n");
}
