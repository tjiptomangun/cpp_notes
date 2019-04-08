#include <stdio.h>

#define MAX_CHAR 16

void cleanup(char buff[], int max_size) {
	int i = 0;
	for (i = 0; i < max_size; i++){
		buff[i] = 0;
	}
}

void reverse0 (char inbuf[], int len){
	int tmp;
	int i;

	for (i = 0; i < len/2; i++){
		tmp = inbuf[i];
		inbuf[i] = inbuf[len - i - 1];
		inbuf[len - i - 1] = tmp;
	}
}
int main (int argc, char **argv) {
	int i = 0;
	int len;
	int ch;
	char buffer [MAX_CHAR];
	cleanup(buffer, MAX_CHAR);

	while((ch = getchar()) != EOF && ch != '\n' && i < MAX_CHAR){
		buffer[i] = ch;
		i ++;
	}
	len = i;

	for (i = 0 ; i < len; i ++){
		putchar(buffer[i]);
	}
	putchar('\n');

	reverse0(buffer, len);
	for (i = 0 ; i < len; i ++){
		putchar(buffer[i]);
	}
	putchar('\n');
}
