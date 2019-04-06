#ifndef _FILECFGPARSER_H_
#define  _FILECFGPARSER_H_ 
#include "parserclass.h"

extern int preprocessfile (FILE *fp, char * tokenlist, PLIST * plist); 
extern int infile_parse (PMINIPARSER parser, PLIST *plist);

#endif
