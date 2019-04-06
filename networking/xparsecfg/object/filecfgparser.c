/**
 * config parser
 * parsing configuration file
 * implements parserclass
 */
#include <string.h>
#include <stdlib.h>

#include "parserclass.h"
#include "filecfgparser.h"

/* keys here . XK_ mean X Key*/
#define XK_L_ANGLE_BRACKET '['
#define XK_R_ANGLE_BRACKET ']'
#define XK_L_PAREN_BRACKET '('
#define XK_R_PAREN_BRACKET ')'
#define XK_ASTERIX '*'
#define XK_NL '\n'
#define XK_EQU '='
#define XK_COMMA ','
#define XK_HASH '#'
/*our language specific parser*/ 
int preprocessfile (FILE *fp, char * tokenlist, PLIST * plist)
{ 
	char buffer[256];
	int token  = -1;
	int  prevtoken = -1; 
	int length = 0;
	PLIST root = newlist ("root"); 
	PLIST lcurr = NULL; /*current active list*/
	PPROPERTY pcurr = NULL; /*current active property */
	* plist = root;

	/* char *keywords[]={"environment", "process",0, "log",0}; */
	token = gettoken (fp, tokenlist, buffer, 256, &length) ;
#define XKW_SECTION_MIN  0
#define XKW_SECTION_ENV  0
#define XKW_SECTION_PROC 1
#define XKW_SECTION_MAX  1 
#define XKW_PROPERTY_MIN 3
#define XKW_PROPERTY_LOG 3
#define XKW_PROPERTY_MAX 3
	while ( token >= 0)
	{
		if (tokenlist[token] != 0)
		{
/*
			fprintf (stdout, "token idx=%d val=%c", 
				token, tokenlist[token]); 
			if (length > 0) 
				fprintf (stdout, "buff = %s", buffer);
			fprintf (stdout, "\n"); 
*/
		
		}
		prevtoken = token;
		switch ( tokenlist[token] )
		{
		case XK_R_ANGLE_BRACKET:
			token = expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, "\n");
			break;
		case XK_L_ANGLE_BRACKET:
			token = expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, "]");
			if (tokenlist[token] != (int)']')
			{
				fprintf (stderr, "parse error");
				return -1;
			}
			trimstring (buffer);
			lcurr = newlist (buffer);
			root->add(root, &lcurr->l_item); 
			break;
		case XK_NL:
			token = expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, tokenlist);
			break; 
		case XK_HASH:/* comment, ignore everything in this line */
			token = expectnexttoken (fp, tokenlist, buffer, 256,
				&length, "\n");
			if (tokenlist[token] != '\n')
			{
				memset (buffer, 0, 256);
				token = prevtoken;
			}
			break; 
		case XK_EQU:
			trimstring (buffer);
			pcurr = newproperty(buffer);
			token = expectnexttoken (fp, tokenlist, buffer, 256,
				&length,"\n#"); 
			if ((token < 0) || (lcurr == 0))
			{
				pcurr->l_item.class.preparedelete(&pcurr->l_item.class);
				free (pcurr); 
				break;
			} 
			trimstring (buffer);
			pcurr->setvalue(pcurr, buffer);
			lcurr->add (lcurr, &pcurr->l_item); 
			break; 
		default: 
			token = gettoken (fp, tokenlist, buffer, 256, &length) ;
			break;
		}
	}
/*
	if (length > 0) 
		fprintf (stdout, "buff = %s", buffer);
	fprintf (stdout, "\n"); 
*/
	return NULL;
}


int infile_parse (PMINIPARSER parser, PLIST *plist)
{
	preprocessfile (parser->input, parser->tokenlist, plist);
	return 0;
}

