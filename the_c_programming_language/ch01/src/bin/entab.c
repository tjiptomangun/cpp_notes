#include <stdio.h>

#define MAX_CHAR 100

int get_line(char buf[], int buff_size, int space_to_tab) {
	int ch;
	int space = 0;
	int i = 0;
	int j = 0;
	int prev = 0;
	while ((ch = getchar()) != EOF && ch != '\n' && i < buff_size) {
		if (prev != ' '){
			space = 0;
		}
		if (ch == ' '){
			space ++;
			if (space == space_to_tab) {
				buf[i++] = '\t';
				prev = '\t';
			}
			else{
				prev = ' ';
			}
		}
		else {
			if (prev == ' '){
				for (j = 0; j < space; j++)
					buf[i ++] = ' ';
			}
			buf[i++] = ch;			
			prev = ch;
		}
	}
	return i;
}

int main (int argc, char **argv) {
	char buffer[MAX_CHAR];
	int len = 0;
	int i;

	while ((len = get_line(buffer, MAX_CHAR, 6))>0 ){
		for (i = 0; i < len; i ++)
			putchar(buffer[i]);
		putchar('\n');
		
	}
}
