#include <stdio.h>
#include <stdlib.h>



int gettoken (FILE *fp, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read)
{
	int ch  = 0;
	int i = 0;
	int j = 0;
	*length_read = 0;
	while ((ch = fgetc (fp)) != EOF)
	{
		i = 0;
		while (tokenlist[i] !=0 )
		{
			if (tokenlist[i] == (char) ch)
			{
				*length_read = j;	
				buff[j] = 0;
				return i;
			}
			i++;
		} 
		if ( j < max_buffsize )	
			buff[j++] = (int) ch; 
	}
	return -1;
}

int option ()
{
	fprintf (stdout, "type:\n"
			"get item_name - to set active item\n"
			"print - print properties of active item\n"
			"q - quit application\n"); 
	return 0;
}
int  main (int argc, char **argv) 
{
	char tokenlist[] = " \nq";
	char buffer[256];
	int lenread = 0;
	int token = 0;
	option();
	while ((token = gettoken 
		(stdin, tokenlist, buffer, 256, &lenread)) != -1)
	{
		switch (tokenlist[token])
		{
		case 'q':
			fprintf (stdout, "danke schon..\n");
			exit (0);
			break;
		case '\n':
			fprintf (stdout, "newline\n");
			fprintf (stdout, "buffer: %s\n", buffer);
			break;
		case ' ':
			fprintf (stdout, "space\n");
			fprintf (stdout, "buffer: %s\n", buffer);
			break;
		}

	}
	exit (0); 
}
