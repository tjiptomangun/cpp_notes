#include <stdio.h>

void squesse(char in[], char pat[], char out[]){
	int i, p, o;
	int fnd;
	i = 0;
	p = 0;
	o = 0;

	while(in[i] != 0){
		fnd = 0;
		p = 0;
		while(pat[p] != 0 && !fnd){
			fnd = fnd || (pat[p] == in[i]);
			p++;
		}
		if (!fnd)
			out[o++] = in[i];
		i++;
	}
}

int main (int argc, char **argv) {
	char out[10];
	int o;
	for (o = 0; o < 10; o++){
		out[o] = 0;
	}
	squesse("abcdef", "cd", out);
	printf("%s\n", out);
}
