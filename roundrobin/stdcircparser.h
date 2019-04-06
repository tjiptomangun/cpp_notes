#ifndef __STDUSERPARSER_H__
#define __STDUSERPARSER_H__

extern int userparse (FILE *fp, char *tokenlist, PLIST *proot); 
extern int stdin_circparse (PMINIPARSER parser, PLIST *plist);
extern int stdin_circparse2 (PMINIPARSER parser, PLIST *plist);


#endif
