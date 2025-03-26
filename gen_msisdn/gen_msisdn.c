#include <stdio.h>

int main (int argc, char **argv) {
	unsigned long long counter = 6630321062237;
	for(int i = 0; i < 2000; i++) {
		fprintf(stdout, "%12llu\n", counter);
		counter += 37;
	}
}
