#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *c1 = calloc(1, 100);
	sprintf(c1, "Terong");

	c1 = realloc(c1, 500);
	fprintf(stdout, "c1 %s\n", c1);

	free(c1);

	return 0;
}
