#include <stdio.h>

unsigned int fun_test(unsigned int d) {
	char arrj_h [10000];
	arrj_h[0] = 0;
	sprintf(arrj_h, "d = %d" , d);
	return d;
}

int main (int argc, char **argv) {
	unsigned int i = 0;
	for (;i< 300000000; i++)
		fun_test(i);
}
