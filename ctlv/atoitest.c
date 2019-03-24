/**
 * $CC -Wall -ggdb3 atoitest.c -o atoitest
 **/
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	int gotcha = 0;
	gotcha = atoi (argv[1]);	
	fprintf (stdout, "you type: %d\n", gotcha);
	exit (EXIT_SUCCESS);
}
