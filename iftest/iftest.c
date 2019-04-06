/**
 * iftest.c test if condition
 * gcc -Wall -ggdb3 iftest.c -o iftest
 */
#include <stdio.h>


int main (int argc, char **argv)
{
	int i = 100;
	int j = 101;
	int k = 102;

	if (i == 101)
	{
		fprintf (stdout, "101\n");
	}
	else if (i+=k-i, i == 101)
	{ 
		fprintf (stdout, "101!=102\n");
	}
	else if (i+=k-i, i == 102)
	{ 
		fprintf (stdout, "whatever\n");
	}
	else
	{ 
		fprintf (stdout, "unmatch\n");
	}


}
