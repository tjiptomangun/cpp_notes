#include<stdio.h>
#include<stdlib.h>

int test_free(char *in) {
	free(in);
	free(in);
	return 0;
}

int main (int argc, char **argv) {
	char *ptr = (char *) calloc(1, 4);
	test_free(ptr);
	return 0;
}
