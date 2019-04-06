#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 2048 
int  (FILE *fpin)
{
	FILE *fpout = NULL;
	unsigned char strLine[BUFFER_SIZE];
	char *marker = "Excerpting match file";
	int markerlen = 22;
	while (fgets (strLine, BUFFER_SIZE, fpin))
	{
		if (!strncmp (strLine, marker, markerlen))
		{

		}
		if (!fpout)
			continue;


	}	

}
int main (int argc, char **argv)
{
	FILE *fp;
	fp = fopen (argv[1], "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file\n");
		exit (EXIT_FAILURE);	
	}
	

	fclose (fp);

}


