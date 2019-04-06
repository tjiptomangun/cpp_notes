#ifndef _FILECFGPARSER_H_
#define  _FILECFGPARSER_H_ 
#include "parserclass.h"

extern int preprocessfile (FILE *fp, char * tokenlist, PLIST * plist); 
extern int infile_parse (PMINIPARSER parser, PLIST *plist);
extern int infile_parse2 (PMINIPARSER parser, void *plist);
typedef LIST configlist;
typedef configlist CONFIGLIST, *PCONFIGLIST; 
extern PCONFIGLIST newconfiglist (char *name);
/**
 * NAME		: filecfgparser 
 * DESCRIPTION	: this class holds structure that is used 
 *		  in parsing config file.
 *		  this class is also responsible for 
 */
#define MAX_INFILENAME 256
typedef struct filecfgparser
{
	struct class class;
	char infilename[MAX_INFILENAME];
	char *tokenlist;
	PCONFIGLIST result;
	int (*parse) (struct filecfgparser *, char *);
	PL_ITEM (* getlitem)(struct filecfgparser *, char *szName);
	char * (*getstringvalue)(struct filecfgparser *, char *, char*);
	int (*getintvalue)(struct filecfgparser *, char *, char*); 
	int (*addproperty) (struct filecfgparser *, char *, char *);
	
} FILECFGPARSER, *PFILECFGPARSER;

extern  PFILECFGPARSER newfilecfgparser (char *classname, char *filename);

#endif
