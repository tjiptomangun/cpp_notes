#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char * justRet(char *outdest, int n, char *in) {
	strcpy(&outdest[n], in);	
	return outdest;
}
int  main(int argc, char **argv) {
	char init[200] = {'f', 'f'};
	char k[4] = {0};
	justRet(init, 5, "10");
	justRet(init, 11, "12");
	justRet(init, 16, "abc");
	printf("%s\n", init);
	return 0; 
}
