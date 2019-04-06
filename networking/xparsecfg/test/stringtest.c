#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trimstring (char *instr)
{
	int len = strlen(instr);
	int first = 0;
	int last = len;
	int k = 0;
	int i = 0;
	if (!len)
		return;

	k = (int)instr[i++];

	while(k != 0)
	{
		if (k != ' ')
			break;	
		k = (int)instr[i++];
	}
	first = i-1; 
	i = len-1;

	do
	{
		k = (int)instr[i--];
		if(k != ' ')
			break;
	}
	while ((k != 0) && (i > first)) ;
	
	last = i+1; 
	len = last - first + 1;
	memcpy (instr, instr + first, len);
	instr[len]  = 0;
	fprintf (stdout, "(%d, %d)\n", first, last); 
}


int main (int argc, char ** argv)
{
	char buff [256];
	memset (buff, 0, 256);
	if (argc < 2)
	{
		fprintf (stderr, "trimtest string\n");
		exit (-1);
	}
	strncpy (buff, argv[1], strlen(argv[1]));
	fprintf(stdout, "before len = %d str=%s\n", (int) strlen(buff), buff); 
	trimstring (buff);
	fprintf(stdout, "after len = %d str=%s\n", (int) strlen(buff), buff);

	exit(0);

}
