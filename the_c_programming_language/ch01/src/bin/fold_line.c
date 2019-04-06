#include <stdio.h>

#define MAX_CHAR 60

/**
 * buff : buffer to store
 * buff_max : buffer maximum length
 */
int get_line(char buff[], int offset, int buff_max, int depth) {
	int ch;
	int sep_idx = -1;
	int i = 0;
	int len = 0;
	

	while((i + offset) < buff_max && (ch = getchar()) != EOF && ch != '\n') {
		if (ch == ' ' || ch == '\t')
			sep_idx = i;
		buff[i] = ch;
		i++;
	}

	len = i;

	// No fold required, line ended with \n or EOF with len < MAX_CHAR
	if (len + offset < buff_max){
		for (i = 0; i < len; i ++)
			putchar(buff[i]); 
		putchar('\n');
			return len;
	}
	// meets buffer maximum
	else {
		// got a separator
		if (sep_idx >= 0){
			for (i = 0; i < sep_idx ; i ++)
				putchar(buff[i]); 
			putchar('\n');
			i ++;
			for (; i < len; i ++)
				putchar(buff[i]); 
			return len + get_line(buff, buff_max - (sep_idx + offset + 1) , buff_max, depth + 1); 
		}
		else{
			for (i = 0; i < len - 1; i ++)
				putchar(buff[i]); 
			putchar('-'); 
			putchar('\n');
			putchar(buff[i]); 
			return (len - 1)  + get_line(buff, 1, buff_max, depth + 1); 
			
		}
	} 
}
	

int main (int argc, char **argv) {
	char buffer[MAX_CHAR];
	while(get_line(buffer, 0, MAX_CHAR, 0) > 0){
	}
}
