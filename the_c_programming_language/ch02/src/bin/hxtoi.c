#include <stdio.h>


int get_line(char s[], int lim){
	int c, i;
	i = 0;
	while ((i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n')) {
		s[i++] = c;
	}

	if (c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

int htoi(char in[]){
	int ret = 0;
	int i = 0;
	if (in[0] == 0 && (in[1] == 'x' || in[1] == 'X'))
		i = 2;

	for(; in[i] != 0;  i++){
		if(in[i] >= '0' && in[i] <= '9'){
			ret <<= 4;
			ret += in[i] - '0';
		}
		else if (in[i] >= 'a' && in[i] <= 'f'){
			ret <<= 4;
			ret += in[i] - 'a';
		}
		else if (in[i] >= 'A' && in[i] <= 'F'){
			ret <<= 4;
			ret += in[0] - 'A';
		}
		else{
			return ret;
		}
	
	}
	return ret;
}

#define MAX_LINE 100
int main(int argc, char **argv) {
	int len;
	char line[MAX_LINE];	

		
	while((len = get_line(line, MAX_LINE)) > 0){ 
		fprintf(stdout, "%d\n", htoi(line));
	}
}
