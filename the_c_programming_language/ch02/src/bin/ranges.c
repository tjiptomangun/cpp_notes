#include <stdio.h>
#include <limits.h>
#include <float.h>

int main (int argc, char **argv) {
	int var = 0;

	char ch;
	unsigned char uch = 0xFF;
	short sh;
	unsigned short ush;
	int sint;
	unsigned int uint;
	long slong;
	unsigned long ulong;
	
	ch = 0x80;	
	printf("calc char min : %d\n", ch);
	ch = 0x7F;	
	printf("calc char max : %d\n", ch);
	printf("calc unsigned char max : %d\n", uch);
	printf("header char min: %d\n", SCHAR_MIN);
	printf("header char max: %d\n", SCHAR_MAX);
	printf("header unsigned char max: %d\n", UCHAR_MAX);

	sh = 0x8000;
	printf("calc short min : %d\n", sh);
	sh = 0x7FFF;
	printf("calc short max : %d\n", sh);
	ush = 0xFFFF;
	printf("calc unsigned short max : %d\n", ush);

	printf("header short min: %d\n", SHRT_MIN);
	printf("header short max: %d\n", SHRT_MAX);
	printf("header unsigned short max: %d\n", USHRT_MAX);

	sint =  0x80000000;
	printf("calc int min : %d\n", sh);
	sint = 0x7FFFFFFF;
	printf("calc int max : %d\n", sh);
	uint  = 0xFFFFFFFF;
	printf("calc unsigned int max : %u\n", uint);

	printf("header int min: %d\n", INT_MIN);
	printf("header int max: %d\n", INT_MAX);
	printf("header unsigned int max: %u\n", UINT_MAX);

	slong =  0x8000000000000000;
	printf("calc int min : %ld\n", slong);
	slong = 0x7FFFFFFFFFFFFFFF;
	printf("calc int max : %ld\n", slong);
	ulong  = 0xFFFFFFFFFFFFFFFF;
	printf("calc unsigned int max : %lu\n", ulong);

	printf("header long min: %ld\n", LONG_MIN);
	printf("header long max: %ld\n", LONG_MAX);
	printf("header unsigned long max: %lu\n", ULONG_MAX);

}
