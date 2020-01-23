/**
 * on how to debug pid and use command file
 * gcc -Wall -ggdb3 loopcon.c -o loopcon
 *
 */
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
int fun2(int v) {
	int x ;
	for (int i = 0; i < 10; i ++) {
		x = x*2;
	}
	return v + x;
}

int fun1(int v) {
	int x = 20;
	int y = 35;
	int z = x * y;
	return z + fun2(v) - v;
	
}

int fun0(int v) {
	if (v > 1000000)
		return fun1(v);
	else if (v < -100000)
		return fun1(v - 2*fun2(v));
	int y = 3 * fun2(v);
	int z = 1 * fun1(v) - 2*fun0(y);
	return y + z; 
}
int main (int argc, char **argv) {
	int i = 2;
	int j = -3;
	while(1) {
		j = fun0(i + j);
		fprintf(stdout, "i = %d j = %d\n", i, j);
		sleep(1);
	}
}
