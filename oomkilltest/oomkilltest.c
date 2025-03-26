/**
 * gcc -Wall -ggdb3 oomkilltest.c o oomkilltest
 */
#include<stdio.h>
#include<stdlib.h>
#define MAX_ITER 160

int main (int argc, char **argv) {
	unsigned char *xPtr[MAX_ITER] = {0};
	for (int i = 0; i< MAX_ITER; i++) {
		printf(" %02d", i);
		xPtr[i] = (unsigned char *) calloc(1, 1024*1024*1024);
		sleep(1); 
		if ((i + 1)%10 == 0) {
			printf("\n");
		}
	}
	printf("write all\n");
	for (int i = 0; i< MAX_ITER; i++) {
		for (int j = 0; j < MAX_ITER; j++) {
			xPtr[i][j] = (unsigned char) (i* MAX_ITER + j);
		}
	}

	while(1) {
		sleep(10);
	}
	
}
