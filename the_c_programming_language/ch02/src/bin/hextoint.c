#include <stdio.h>
/*
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
			break;
		}
	
	}
	return ret;
}
*/
int htoi() {
	int c;
	int ret = 0;
	while (((c = getchar()) != '\0') && (c != EOF) && (c != '\n')){
		if(c >= '0' && c <= '9'){
			ret <<= 4;
			ret += c - '0';
		}
		else if (c >= 'a' && c <= 'f'){
			ret <<= 4;
			ret += c - 'a';
		}
		else if (c >= 'A' && c <= 'F'){
			ret <<= 4;
			ret += c - 'A';
		}
		else if (!((c == 'x' || c == 'X') && ret == 0))
			return ret;
	}
	return ret;
}
int main(int argc, char **argv) {
	fprintf(stdout, "%d\n", htoi());
}
