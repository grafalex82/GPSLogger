#include <stdio.h>
#include <string.h>

typedef unsigned char uint8;
typedef unsigned int uint32;

// This is some kind of a unit test for float value print helper. Code under the test is injected into a test function below via simple copy/paste from FloatPrinter constructor.
// This allows executing the code right at C++-in-browser service (such as http://cpp.sh)
// I just did not want to set up a development toolchain, create a project file, deal with external libraries, do a dependency injection into tested class, etc :)

void test(const char * expectedValue, float value, uint8 width, bool leadingZeros = false, bool alwaysPrintSign = false)
{
    char buf[9];
    uint8 pos;

    printf("Printing %f... ", value);

////////////////////////////////////////////////////////
// Begin copy from FloatPrinter
////////////////////////////////////////////////////////

    // reserve a space for sign
    uint8 minpos = 0;
    if(alwaysPrintSign || value < 0)
        minpos++;

	// absolute value to print, deal with sign later
	float v = value;
	if(v < 0)
	    v = 0. - v;
	
	// floating point position will depend on the value
	uint8 precision = 0;
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

	uint32 iv = v + 0.5; // we will be operating with integers

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

////////////////////////////////////////////////////////
// End copy from FloatPrinter
////////////////////////////////////////////////////////

    if(strcmp(expectedValue, buf+pos) == 0)
    {
        printf("%s - PASSED\n", buf+pos);
    }
    else
    {
        printf("%s - FAILED\n", expectedValue);
        printf("Got: %s\n", buf+pos);
        
        printf("Buffer: ");
	    for(int i=0; i<9; i++)
    	    printf("%2x ", buf[i]);
    	printf("\npos=%d\n\n", pos);
    }
}

int main()
{
    test("0", 0., 4);

    test("0.10", 0.1, 4);
    test("0.23", 0.23, 4);
    test("4.00", 4., 4);
    test("5.60", 5.6, 4);
    test("7.89", 7.89, 4);
    test("1.23", 1.234, 4);
    test("56.8", 56.78, 4);
    test("56.8", 56.78, 5);
    test("123", 123.4, 4);
    test("568", 567.8, 5);
    test("12345", 12345., 6);

    test("-0.10", -0.1, 5);
    test("-0.23", -0.23, 5);
    test("-4.00", -4., 5);
    test("-5.60", -5.6, 5);
    test("-7.89", -7.89, 5);
    test("-1.23", -1.234, 5);
    test("-56.8", -56.78, 5);
    test("-56.8", -56.78, 6);
    test("-123", -123.4, 5);
    test("-568", -567.8, 6);
    test("-12345", -12345., 7);
}
