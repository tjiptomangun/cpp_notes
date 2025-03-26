#include <stdio.h>
#include <stdlib.h>
int f0() {
	return 1;
}
int f1() {
	return 1;
}
int f2(int howmany) {
	if (howmany) {
		return howmany + f2(howmany - 1);
	}
	return 0;
}
int main (int argc, char **argv) {

	f1();
	f1();
	f2(30); 
	exit(0);
}
