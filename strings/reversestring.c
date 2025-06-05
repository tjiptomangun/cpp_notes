#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * gcc -Wall -ggdb3 reversestring.c -o reversestring
 */

/**
 * assume out large enough
 */
char *reversestring(char *in, char *out) {
    int len = strlen(in);
    int isOdd = len % 2;
    int i;
    int mid = len/2;
    for (i = 0; i < mid; i++) {
        *(out + i) = *(in+(len -1- i));
        *(out +len -1- i) = *(in + i);
    }
    if(isOdd) {
        *(out + mid) = *(in + mid);
    }
    return out;

}
int main(int argc, char **argv) {
	char *out;
	if(argc < 2) {
		fprintf(stderr, "usage: %s <string_to_reverse>\n", argv[0]);
		exit(1);
	}
	int len = strlen(argv[1]);
	out = (char *) calloc(len, 1);
	fprintf(stdout, "%s\n", reversestring(argv[1], out));
	free(out); 
}
