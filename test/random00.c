/**
 gcc -Wall -ggdb3 random00.c -o random00
 */
#include <stdlib.h>
#include <stdio.h>
unsigned int seed = 20;
int randInt(unsigned int max) {
	return rand_r(&seed) % max;
}
int main (int argc, char **argv) {
	for(int i = 0; i < 10; i++) {
		fprintf(stdout, "%d\n", randInt(20));
	}

}
