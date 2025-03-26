#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char *getNextString(char *in, char *out) {
	char k[200] = {0};
	strcat(k, " ");
	strcat(k, in);
	strcat(out, k);
	return out;
}
int main (int argc, char**argv) {
	char out[500] = {0};

	getNextString("one", out);
	getNextString("two", out);
	getNextString("three", out);
	printf("%s\n", out);

}
