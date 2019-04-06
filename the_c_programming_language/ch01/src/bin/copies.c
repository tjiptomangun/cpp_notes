#include <stdio.h>

int main (int argc, char **argv) {
	int c;

	while((c = getchar()) != EOF) { 
		if (c == ' '){
			putchar(c);
			do {
				c = getchar();
			}while(c == ' ');
		}
		if (c != EOF)
			putchar(c);
	}
}
