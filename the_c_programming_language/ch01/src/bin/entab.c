#include <stdio.h>
#include <stdlib.h>
#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#define MAX_CHAR 100

int get_line(char buf[], int buff_size, int space_to_tab) {
	int ch;
	unsigned short space = 0;
	unsigned short i = 0;
	int j = 0;
	int prev = 0;
	while ((ch = getchar()) != EOF && ch != '\n' && i < (buff_size - 1)) {
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
				for (j = 0; j < space && i < (buff_size - 1); j++)
					buf[i ++] = ' ';
			}
			if ( i < (buff_size - 1)) {
				buf[i++] = ch;			
				prev = ch;
			}
		}
	}
	return i;
}

int main (int argc, char **argv) {
	char buffer[MAX_CHAR] = {0};
	int len = 0;
	int i;

	while ((len = get_line(buffer, MAX_CHAR, 6))>0 ){
		for (i = 0; i < min(len, sizeof(buffer)); i ++){
			putchar(buffer[i]);
		}
		putchar('\n');
		
	}
}
