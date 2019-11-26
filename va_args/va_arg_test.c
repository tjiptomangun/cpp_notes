#include <stdarg.h>
#include <stdio.h>

int sum(int, ...);

int main() {
	printf("Sum of 15 and 56 = %d\n", sum(2, 15, 56));
	return 0;
}

int sum(int num_args, ...){
	int val = 0;
	va_list ap;//va_list is variable

	int i;

	va_start(ap, num_args);/*va_start is a macro, requires the last fixed parameter to get the address*/
	for (i = 0; i < num_args; i++){
		val += va_arg(ap, int);/*va_arg is a macro*/
	}
	va_end(ap);/*va_end is a macro*/
	return val;
		
}
