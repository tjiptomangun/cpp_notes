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
#include "stduserparser.h" 
 
int main (int argc , char **argv)
{
	FILE *fp = NULL; 
	char tokenlist [] ="[]#\n=";
	PLIST root = 0; 
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
	pfile = newminiparser (fp, tokenlist, infile_parse);
	
	if (root )
	{
		fprintf (stdout, "num_root_elmt : %d \n", root->count);
	}
	pfile->parse (pfile,&root);
	fclose (fp); 
	
	pstdio = newminiparser (stdin, " \n", stdin_parse);	
	pstdio->parse (pstdio,&root);

	root->l_item.class.delete(&root->l_item.class);


	exit (0);
}
