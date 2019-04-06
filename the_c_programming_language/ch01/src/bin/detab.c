#include <stdio.h>


int main (int argc, char **argv) {
	int ch;
	int tab_to_space = 6;
	int j = 0;

	while((ch = getchar())!=EOF){
		if (ch == '\t'){
			for(j = 0; j < tab_to_space; j++)
				putchar(' ');
		}
		else{
			putchar(ch);
		}
	}
}
