/* xparsecfg 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c parserclass.c filecfgparser.c stduserparser.c -Wall -ggdb3 -o xparsecfg 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "filecfgparser.h"
#include "stdfileparser.h" 
 
int main (int argc , char **argv)
{
	FILE *fp = NULL; 
	char tokenlist [] ="[]#\n=";
	PFILECFGPARSER root ;
	PMINIPARSER pfile, pstdio; 
	if (argc < 2)
	{
		fprintf (stderr, "usage %s filename\n", argv[0]);
		exit (-1); 
	}

	fp = fopen (argv[1], "r");
	if (!fp)
	{
		fprintf (stderr,"err file %s\n", argv[1]);
		exit (-1);
	}
	root = newfilecfgparser ("test", argv[1]);
	if (!root)
	{
		fprintf (stdout, "failed creating filecfgparser\n");
		exit (EXIT_FAILURE);
	}
	pfile = newminiparser (fp, tokenlist, infile_parse2);
	
	pfile->parse (pfile,(void *)root);
	fclose (fp); 
	
	pstdio = newminiparser (stdin, " \n", stdfile_parse);
	pstdio->parse (pstdio,(void *)root);

	root->class.delete(&root->class);


	exit (0);
}
