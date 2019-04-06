#include <stdio.h>

int main (int argc, char **argv){
	int nspace;
	int ntabs;
	int nline;

	int c;

	nspace = ntabs = nline = 0;

	while((c = getchar()) != EOF){
		if (c == ' ')
			nspace ++;
		else if (c == '\t')
			ntabs ++;
		else if (c == '\n')
			nline ++;
	}

	printf("space tabs line\n");
	printf("----- ---- ----\n");
	printf("%05d %04d %04d\n", nspace, ntabs, nline);
}
