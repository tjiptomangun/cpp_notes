#include "pmnodes.h"

PLIST root = 0;
int pm_readnodes ()
{
	FILE * fp = NULL;
	char tokenlist [] = "[]#\n=";
	PMINIPARSER pfile;
	pfile  = newminiparser (fp, NULL, infile_parse2); 
	pfile->parse (pfile, &root);
	fclose (fp);
}


