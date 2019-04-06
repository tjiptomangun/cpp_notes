/**
 * writehell - writes like hell
 * gcc -Wall -ggdb3 writehell.c -D_LARGEFILE64_SOURCE -o  writehell
 **/

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void usage (char *pName)
{
	fprintf (stderr, "usage : %s filename num_write_presleep num_iter sleep\n", pName);
}
int main (int argc, char **argv)
{
	int i = 0;
	int j = 0;
	int iter = 0;
	int writerept = 0;
	int msleep = 0;
	char buff []  = "Hello Buzz World. Buzzzzzwooooooorlllddddzzz \06\07\09\03\n";
	char tmp []  = "Get by it Hello Buzz World. Buzzzzzwooooooorlllddddzzz \06\07\09\03 Stosd stosw mbsw\n";
	int bufflen = strlen (buff);
	int tmplen = strlen (tmp);
	FILE *pTraceFile;
	if (argc < 5)
	{
		usage (argv[0]);
		exit (EXIT_FAILURE);
	}
	if ((pTraceFile = fopen64 (argv[1], "ab")) == (FILE *) NULL)
	{
		perror ("\nTraceOpen");
		exit (EXIT_FAILURE);
	}
	writerept= atoi (argv[2]);
	iter = atoi (argv[3]);
	msleep = atoi (argv[4]); 
	setvbuf (pTraceFile, NULL, _IOLBF, BUFSIZ);
	while (1)
	{
		fprintf (pTraceFile, "Init Buzz........\n");
		fwrite (buff, 1, bufflen+1, pTraceFile);
		fwrite (tmp, 1, tmplen+1, pTraceFile); 
		i ++;
		if ((i%writerept)== 0)
		{
			usleep (msleep*1000);
			i=0;
			j++;
			if (j == iter)
				break;
		}
	}
	fclose (pTraceFile); 
	exit (EXIT_SUCCESS);
}
