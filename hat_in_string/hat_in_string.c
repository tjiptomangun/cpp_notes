#include <stdio.h>

int main (int argc, char **argv) {
	char * a = "Hello World!";
	char * b = "rld";
	int k = *a ^ *b;

	fprintf(stdout, "k = %d\n", k);
}
