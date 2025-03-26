#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main ( int argc, char **argv)
{
	char * g;
	g = (char *) malloc(10);
	g[12] = 0;
//	while(1)
	{
		sleep(1);
	}	
	free(g);
	return 0;
}
