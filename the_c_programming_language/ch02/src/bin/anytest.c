#include <stdio.h>
int anytest(char in[], char pat[]){
	int i, p;
	int fnd;
	i = 0;
	p = 0;

	while(in[i] != 0){
		fnd = 0;
		p = 0;
		while(pat[p] != 0 && !fnd){
			fnd = fnd || (pat[p] == in[i]);
			p++;
		}
		if (fnd)
			return i;
		i++;
	}
	return -1;
}

int main (int argc, char **argv) {
	int result;
	result = anytest("abcdef", "cd");
	printf("%d\n", result);
}
