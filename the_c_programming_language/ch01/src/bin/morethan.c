#include <stdio.h>

int main (int argc, char **argv) {
	int c;
	int i; 

	i = 0;
	while((c = getchar()) != EOF){
		if (c == '\n'){
			if (i > 80)
				putchar(c);
			i = 0;
		}
		else {
			if(i > 80)
				putchar(c);
			i++;
		}
	}
}
