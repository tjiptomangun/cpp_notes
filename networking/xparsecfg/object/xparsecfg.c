/* xparsecfg 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c -Wall -ggdb3 -o xparsecfg
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "filecfgparser.h"
#include "stduserparser.h" 

int cleanup ()
{
	FILE *fnull = fopen ("/dev/null", "w"); 
	int c = 0;
	while ((c = fgetc (stdin)) != 0x0a)
		;
	ungetc(c, stdin);
	fclose (fnull);	
	return 0;
}



int printlistname(PLIST plist)
{
	PL_ITEM pl_item;
	if (!plist)
		return 0;
	fprintf (stdout, "print list member of [%s]\n", plist->l_item.class.name);
	while ((pl_item = plist->take(plist)) != NULL)
	{
		fprintf (stdout, "l_item [%s]", pl_item->class.name);
		pl_item->class.preparedelete(&pl_item->class);
		free (pl_item);
		pl_item = NULL;
	}
	fprintf (stdout, "\n");
	return 0;
} 

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
	/*
	root->l_item.class.preparedelete(&root->l_item.class);
	*/
	free(root);
	exit (0);
}
