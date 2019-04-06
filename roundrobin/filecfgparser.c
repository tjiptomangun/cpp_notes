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
#define XK_DOLLAR '$'
#define XK_SLASH '/'
#define XK_COLON ':'
/*our language specific parser*/ 
static int __configlist_writetofile (PCONFIGLIST list, FILE *fp)
{

	PL_ITEM pl_item = NULL;
	if (strcmp (list->l_item.class.name, "root"))
		fprintf (fp,"[%s]\n", list->l_item.class.name);
	pl_item = list->head;
	while (pl_item)
	{
		pl_item->class.writetofile (&pl_item->class, fp);	
		pl_item = pl_item->next;
	}
	return 0; 
}

void __configlist_ctor (PCONFIGLIST list, char *name)
{
	__list_ctor (list , name); 
	list->l_item.class.writetofile = 
		(int (*) (PCLASS,FILE *)) (__configlist_writetofile);
}

PCONFIGLIST newconfiglist (char *name)
{
	PCONFIGLIST pclist = (PCONFIGLIST) calloc (1, sizeof (CONFIGLIST));
	if (pclist)
	{
		__configlist_ctor (pclist, name);
	}
	return pclist;
}

int preprocessfile (FILE *fp, char * tokenlist, PCONFIGLIST * plist)
{ 
	char buffer[256];
	int token  = -1;
	int  prevtoken = -1; 
	int length = 0;
	PCONFIGLIST root = newconfiglist ("root"); 
	PLIST lcurr = NULL; /*current active list*/
	PPROPERTY pcurr = NULL; /*current active property */
	PL_ITEM pl_item = NULL;
	* plist = root;

	/* char *keywords[]={"environment", "process",0, "log",0}; */
	token = stream_gettoken (fp, tokenlist, buffer, 256, &length) ;
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
			token = stream_expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, "\n");
			break;
		case XK_L_ANGLE_BRACKET:
			token = stream_expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, "]");
			if (tokenlist[token] != (int)']')
			{
				fprintf (stderr, "parse error");
				return -1;
			}
			trimstring (buffer);
			lcurr = newconfiglist (buffer);
			root->add(root, &lcurr->l_item); 
			break;
		case XK_NL:
			prevtoken = token;
			token = stream_expectnexttoken (fp, tokenlist, buffer, 256, 
				&length, tokenlist);
			if (tokenlist [token] != XK_NL) 
				break;
			else if (!length)
				break;
			else if (!strlen(trimstring(buffer)))
				break;
			if (!lcurr)
				break;
			
			pl_item = (PL_ITEM) newl_item (buffer);
			lcurr->add (lcurr, pl_item); 
				
			break; 
		case XK_HASH:/* comment, ignore everything in this line */
			token = stream_expectnexttoken (fp, tokenlist, buffer, 256,
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
			token = stream_expectnexttoken (fp, tokenlist, buffer, 256,
				&length,"\n#"); 
			if ((token < 0) || (lcurr == 0))
			{
				pcurr->l_item.class.delete(&pcurr->l_item.class); 
				break;
			} 
			trimstring (buffer);
			pcurr->setvalue(pcurr, buffer);
			lcurr->add (lcurr, &pcurr->l_item); 
			break; 
		default: 
			token = stream_gettoken (fp, tokenlist, buffer, 256, &length) ;
			break;
		}
	}
/*
	if (length > 0) 
		fprintf (stdout, "buff = %s", buffer);
	fprintf (stdout, "\n"); 
*/
	return 0;
}

/**
 * NAME		: infile_parse
 * DESCRIPTION	: parsing parser input file 
 * INPUT	 - parser
 *		   input file and token list used to parse is
 *		   supplied by this parameter
 * OUTPUT	 - plist
 *		   output of this parsing
 *		
*/
int infile_parse (PMINIPARSER parser, PLIST *plist)
{
	preprocessfile (parser->input, parser->tokenlist, plist);
	return 0;
}
/*
 * NAME		: infile_parse2
 * DESCRIPTION	: parsing parser input file, with token list is 
 *		  predefined for this function to be "[]#\n=" which
 *		  are preprocess file understand.
 * INPUT	  - parser
 *		    input file is supplied by this parameter.
 * OUTPUT	: plist
 *		  output of this parsing. This function will
 *		  cast it as PFILCFGPARSER
 */
//int infile_parse2 (PMINIPARSER parser, PLIST *plist)
int infile_parse2 (PMINIPARSER parser, void *inout)
{
	PFILECFGPARSER pfile =  (PFILECFGPARSER) inout;
	if (!pfile)
		return  -1; 
	preprocessfile (parser->input, "[]#\n=", &pfile->result);
	return 0;
}
/*
 * NAME		: __filecfgparser_parse
 * DESCRIPTION	: parser
 * RETURN	: 0,   success
 *		  -1, error opening file, check errno 
 *		  others, error parsing file
 */
int __filecfgparser_parse(PFILECFGPARSER parser, char *infilename)
{
	FILE *fp ;
	int ret;
	strncpy(parser->infilename, infilename, MAX_INFILENAME); 
	fp = fopen (parser->infilename, "w");
	if (!fp)
		return -1;
	ret = preprocessfile (fp, "[]#\n=", &parser->result);
	fclose (fp);
	return 0;
}

static int __filecfgparser_delete (PFILECFGPARSER parser)
{
	parser->result->l_item.class.delete (&parser->result->l_item.class);
	__class_delete (&parser->class);
	return 0; 
}
/**
 * NAME		: __filecfgparser_getlitem
 * DESCRIPTION	: get list item of filecfgparser
 *
 **/
static PL_ITEM __filecfgparser_getlitem (PFILECFGPARSER pfile, char *szName)
{
	if (!pfile->result)
		return NULL;
	return pfile->result->getname (pfile->result, szName);
}
/*
 * NAME		: __filecfgparser_getstringvalue
 * INPUT 	: section_name, name of section, if null or zero length then 
 *		  this should be considered as top paramter in list which is 
 *		  not part of any section.
 *		  paramname, name of parameter
 * RETURNS	: string value of section's parameter of last parser file
 *
*/
static char * __filecfgparser_getstringvalue (PFILECFGPARSER pfile, char *section, char *paramname)
{
	PL_ITEM litem;
	PPROPERTY prop;
	PLIST list;
	if (!pfile->result)
		return NULL;
	if (! (section && strlen(section)))
	{
		litem = pfile->result->getname (pfile->result, paramname);
		if (!litem)
			return NULL; 
		prop = (PPROPERTY) litem;
		return prop->value;
	}
	litem = pfile->result->getname (pfile->result, section);
	if (!litem)
		return NULL;
	if (litem->class.type != CLASS_LIST)
		return NULL;
	list = (PLIST) litem;
	litem = pfile->result->getname (list, paramname);
	if (!litem)
		return NULL;
	prop = (PPROPERTY) litem;
	return prop->value;
}

/**
 * NAME		: __filecfgparser_getintvalue
 *
 */
/*
 * NAME		: __filecfgparser_getintvalue
 * INPUT 	: section_name, name of section, if null or zero length then 
 *		  this should be considered as top paramter in list which is 
 *		  not part of any section.
 *		  paramname, name of parameter
 * RETURNS	: int value of section's parameter of last parser file
 *		  0 will be the default value
 *
*/

static int __filecfgparser_getintvalue (PFILECFGPARSER pfile, char *section, char *paramname)
{
	PL_ITEM litem;
	PLIST list;
	PPROPERTY prop;
	if (!pfile->result)
		return 0;
	if (! (section && strlen(section)))
	{
		litem = pfile->result->getname (pfile->result, paramname);
		if (!litem)
			return 0; 
		prop = (PPROPERTY) litem;
		return atoi (prop->value);
	}
	litem = pfile->result->getname (pfile->result, section);
	if (!litem)
		return 0;
	if (litem->class.type != CLASS_LIST)
		return 0;
	list = (PLIST) litem;
	litem = pfile->result->getname (list, paramname);
	if (!litem)
		return 0;

	prop = (PPROPERTY) litem;
	return atoi (prop->value);
}

/**
 * NAME		: __filecfgparser_addproperty
 * DESCRIPTION	: add a property to p result 
 */
static int __filecfgparser_addproperty (PFILECFGPARSER p, char * name, char *value)
{
	PPROPERTY pnew = newproperty2 (name, value);
	if (!p->result)
		return -1;
	if (!pnew)
		return -1;
	p->result->add (p->result, &pnew->l_item);
	return 0;
}
/**
 * NAME		: __filecfgparser_writetoinfile
 * DESCRIPTION	: Write list to a file which name defined in p->infilename 
 * 		  if fx is exists then it will be written there in spite of
 *		  p->infilename
 */
static int __filecfgparser_writetoinfile(PFILECFGPARSER p, FILE *fx)
{
	FILE *fp;
	if (!p->result)
		return 1;
 	if (fx)
	{
		fp = fx;
	}
	else
	{
		if (!strlen (p->infilename))
			return 1;
		fp = fopen (p->infilename, "w");
		if (!fp)
			return 1;
	}
	p->result->l_item.class.writetofile (&p->result->l_item.class, fp);
	if (!fx)
	{
		fclose (fp);
	}
	
	return 0;
} 
static int __filecfgparser_ctor (PFILECFGPARSER pfile, char *name, char *filename)
{

	__class_ctor (&pfile->class, name); 
	pfile->result= NULL;
	memset (pfile->infilename, 0, MAX_INFILENAME); 
	strncpy (pfile->infilename, filename, MAX_INFILENAME);
	pfile->parse = __filecfgparser_parse;
	pfile->class.delete = 
		(int (*) (PCLASS))(__filecfgparser_delete);
	pfile->getlitem = __filecfgparser_getlitem;
	pfile->getstringvalue = __filecfgparser_getstringvalue;
	pfile->getintvalue = __filecfgparser_getintvalue; 
	pfile->class.writetofile= 
		(int (*) (PCLASS, FILE *))(__filecfgparser_writetoinfile);
	pfile->addproperty = __filecfgparser_addproperty;
	return 0;
}

/*
 * NAME		: newfilecfgparser
 * DESCRIPTION	: create a new filecfgparser;
 *
 */
PFILECFGPARSER  newfilecfgparser (char *classname, char *filename)
{
	PFILECFGPARSER pfile = (PFILECFGPARSER) calloc (1, sizeof (FILECFGPARSER));
	if (pfile)
		__filecfgparser_ctor (pfile, classname, filename);
	return pfile; 
}


