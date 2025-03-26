/**
 * $CC -Wall -ggdb3 unarytest.c -o unarytest
 **/
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char **arg)
{
	int initial  = 101;
	fprintf (stdout, "initial entry value : %d\n", initial);
	fprintf (stdout, "initial ++: %d\n", initial ++);
	fprintf (stdout, "initial after unary operation : %d\n", initial);
	fprintf (stdout, "++initial : %d\n", ++initial );
	fprintf (stdout, "initial after unary operation : %d\n", initial);
	fprintf (stdout, "initial --: %d\n", initial --);
	fprintf (stdout, "initial after unary operation : %d\n", initial);
	fprintf (stdout, "--initial : %d\n", --initial );
	fprintf (stdout, "initial after unary operation : %d\n", initial);
	exit (EXIT_SUCCESS);
}
