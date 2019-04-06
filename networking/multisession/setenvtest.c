#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parserclass.h"
#include "filecfgparser.h"

int main (int argc, char **argv)
{
	FILE *fp = NULL;
	PMINIPARSER pfile;
	char	tokenlist [] = "[]#\n=";
	PLIST	root = 0;
	PLIST	curr = 0;
	PPROPERTY pprop = 0; 
	char newenv [258];
	

	fp = fopen ("../dat/XLAdmAgent.ini", "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file ../dat/XLAdmAgent.ini");
		exit (1);
	}
	pfile = newminiparser (fp, tokenlist, infile_parse);
	pfile->parse (pfile, &root);
	fclose (fp); 

	curr = (PLIST) root->takename (root, "environment"); 
	if (curr)
	{
		while ((pprop = (PPROPERTY) curr->take (curr)))
		{
			if (parse_env_str (pprop->value, newenv, 258))
			{
				setenv (pprop->l_item.class.name, newenv, 1); 
				fprintf (stdout, "newenv %s = %s\n",
					pprop->l_item.class.name, newenv);
			}
		} 
	} 
	root->l_item.class.preparedelete (&root->l_item.class);
	system ("xparsecfg");
	free (root);
	exit (0);

}	

