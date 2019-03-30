#include <stdio.h>
/*
 * $CC -Wall -ggdb3 fortest.c -o fortest
 */
int main (int argc, char **argv)
{
	int i = 0, j = 100;
	for (i = 0; i < j ; i++)
	{
		if ((i%10)==0)
		{
			fprintf (stdout, "\n");
			continue;
		}
		fprintf (stdout, "%03d ", i); 
	}
	fprintf (stdout, "\n"); 
	exit (0);
}
