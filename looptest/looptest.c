#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int i = 0, j  =0;
	for (i = 0, j = 0; i<100; ++i, j++)
	{
		fprintf (stdout, "i = %d, j = %d \n", i, j);

	}
	exit (EXIT_SUCCESS);
}
