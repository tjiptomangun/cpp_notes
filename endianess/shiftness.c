#include <stdio.h>
#include <stdlib.h>
/**
 * shiftness.c
 * shift
 * $CC -Wall -ggdb3 shiftness.c -o shiftness
 **/

int main (int argc, char **argv)
{
	int k = 1;
	int i = 0;

	for (i = 0; i<30 ; i++)
	{
		fprintf (stdout, "%08X    %d\n", k, k);
		k<<=1;	
	}

	exit (EXIT_SUCCESS);

}
