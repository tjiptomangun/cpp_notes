#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * $CC hexcheck.c -Wall -ggdb3 -o hexcheck
 *
 **/
int hexCheck (char c)
{
	if (c >='a' && c<= 'f')
		return 0;
	if (c >='A' && c<= 'F')
		return 0;
	if (c >='0' && c<= '9')
		return 0;
	return c;

}
int hexCheckString(char *in)
{
	int len = strlen(in);
	int i = 0;
	int found =0;
	for (i = 0; i<len; i++)
	{
		if (hexCheck(in[i]))
		{
			fprintf (stderr, "FAIL [%d] %c\n", i, in[i]);
			found+=1;
		}

	}
	return found;
}
int main (int argc, char **argv)
{
	int i = 0;
	
	if (argc <2)
	{
		fprintf (stdout, "usage: %s hexbin....\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	for (i = 1; i < argc; i++)
	{
		if (hexCheckString (argv[i]))
		{
			fprintf (stdout, "that was arg[%d]\n",i);
		} 
	}
	exit (EXIT_SUCCESS);
}
