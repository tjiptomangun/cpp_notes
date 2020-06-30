#include <stdio.h>

int main (int argc, char **argv) {
	char a  = 1;
	char b  = 2;
	char out = 0;
	out = a;
	
	for(int i= 0; i < 7; i++){
		out = out | (1 << 0);
		fprintf(stdout, "a = %02X out = %02X\n", a, out);	
	}
	out = b;
	fprintf(stdout, "next\n");
	for(int i= 0; i < 7; i++){
		out = out | (1 << 0);
		fprintf(stdout, "a = %02X out = %02X\n", a, out);	
	}

	

}
