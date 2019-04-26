#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	unsigned char in[] = {0x1a, 0xd2};
	unsigned char *end;
	int ret = 0;
	
	
	ret = strtol(in, &end, 10);

	printf ("out: %d\n", ret);
}
