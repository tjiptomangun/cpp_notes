/* xparsecfg 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c -Wall -ggdb3 -o xparsecfg
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_identation (int ident)
{
	int i;
	for (i = 0; i < ident; i++)
		fprintf (stdout, "\t"); 
} 

void trimstring (char *instr)
{
	int len = strlen(instr);
	int first = 0;
	int last = len;
	int k = 0;
	int i = 0;
	if (!len)
		return;

	k = (int)instr[i++];

	while(k != 0)
	{
		if (k != ' ')
			break;	
		k = (int)instr[i++];
	}
	first = i-1; 
	i = len-1;

	do
	{
		k = (int)instr[i--];
		if(k != ' ')
			break;
	}
	while ((k != 0) && (i > first)) ;
	
	last = i+1; 
	len = last - first + 1;
	memcpy (instr, instr + first, len);
	instr[len]  = 0;
	fprintf (stdout, "(%d, %d)\n", first, last); 
}

#define MAX_NAME_LENGTH	40 
/*our language specific data structure*/
/*section, property, value*/ 
#define CLASS_CLASS 0
#define CLASS_L_ITEM 1
#define CLASS_LIST 2
#define CLASS_PROPERTY 3

typedef struct class 
{
	struct  class *this;
	char    name[MAX_NAME_LENGTH];
	int	type;
	int (*preparedelete) (struct class *);/* cleaning up resources */
	int (*printattributes) (struct class *, int);
}STRUCT_CLASS, CLASS, *PSTRUCT_CLASS, *PCLASS;

static int __class_preparedelete(PCLASS p)
{
	memset (p->name, 0, MAX_NAME_LENGTH);
	p->this = NULL;
	
	return 0;
}

static int __class_printattributes(PCLASS p, int ident)
{
	print_identation (ident);
	fprintf (stdout, "class.name : %s", p->name);
	fprintf (stdout, "\n");
	return 0;
}

PCLASS newclass (char *name)
{
	PCLASS pclass = (PCLASS) calloc (1, sizeof (CLASS));
	pclass->this = pclass;
	pclass->type = CLASS_CLASS;
	strncpy (pclass->name, name, MAX_NAME_LENGTH); 
	pclass->preparedelete = __class_preparedelete;
	pclass->printattributes = __class_printattributes;
	return pclass;	
}

typedef struct l_item
{
	struct class class;
	struct l_item *next;
}L_ITEM, *PL_ITEM;

static int __l_item_preparedelete (PL_ITEM p)
{
	__class_preparedelete ((PCLASS) p); 
	p->next = 0;
	return 0;
}
static int __l_item_printattributes(PL_ITEM p, int ident)
{
	return __class_printattributes(&p->class, ident); 
}

typedef struct list
{
	L_ITEM     l_item;
	PL_ITEM    head;
	PL_ITEM    tail;
	int          count;
	void (*add) (struct list *, PL_ITEM);
	PL_ITEM (*take) (struct list *);
	PL_ITEM (*takename) (struct list *, char *); 
}LIST, *PLIST;

static int __list_printattributes(PLIST p, int ident)
{
	PL_ITEM pl_item = NULL;
	__l_item_printattributes(&p->l_item, ident);
	pl_item = p->head;
	while (pl_item)
	{
		pl_item->class.printattributes(&pl_item->class, ident+1);
		pl_item = pl_item->next;
	} 
	fprintf (stdout, "\n");
	return 0;
}

/**
 * NAME		: __list_add
 * DESCRIPTION	: static function that add time to list class 
 */
static void __list_add (PLIST plist, PL_ITEM add)
{
	if (plist->head == NULL)
	{
		plist->head = add;
		plist->tail = plist->head; 
		plist->count++;
	}
	else
	{
		plist->tail->next = add;
		plist->tail = add; 
		plist->count++;
	}
	plist->tail->next = NULL;
}

/**
 * NAME		: __list_take
 * DESCRIPTION	: take head
 */
static PL_ITEM __list_take (PLIST plist)
{
	PL_ITEM ret;
	if (plist->head)
	{
		ret = plist->head;
		plist->head = plist->head->next;
		plist->count --; 
		return ret;
	}
	else
	{
		return NULL;
	}
} 

/**
 * NAME 	: __list_clean
 * DESCRIPTION	: clean up the list
 */
static int __list_preparedelete (PLIST plist)
{
	PL_ITEM curr;
	__l_item_preparedelete ((PL_ITEM) plist);
	while (( curr = plist->take(plist)))
	{
		curr->class.preparedelete (&curr->class);
		plist->count --;
	}
	return 0;
}

/**
 * NAME		: __list_take_name
 * DESCRIPTION	: take member element of name xx
 */
static PL_ITEM __list_take_name(PLIST plist, char* name)
{
	PL_ITEM curr = plist->head;
	PL_ITEM prev = NULL;
		
	while (curr)
	{ 
		if (!strncmp (name, curr->class.name, MAX_NAME_LENGTH))
		{
			if (prev)
			{
				prev->next = curr->next;
				curr->next = NULL;
			}
			else
			{
				plist->head = curr->next;
				curr->next = NULL;	
			}
			return curr;
		}
		prev = curr;	
		curr = curr->next;
	} 
	return NULL;
}

typedef struct property
{
	L_ITEM l_item ;
	char value[256]; 
	int (*setvalue) (struct property *, char*);
}PROPERTY, *PPROPERTY;

static int __property_preparedelete (PPROPERTY p)
{
	memset (p->value, 0, 256);
	return 0;
}
static int __property_setvalue (PPROPERTY p, char *value)
{
	strncpy (p->value, value, 256);
	return 0;
}
static int __property_printattributes(PPROPERTY p, int ident)
{
	__l_item_printattributes(&p->l_item, ident);	
	print_identation (ident);
	fprintf (stdout, "property.value : %s", p->value);
	fprintf (stdout, "\n");
	return 0;
}

PL_ITEM newl_item (char *name)
{
	PL_ITEM pl_item = (PL_ITEM) calloc (1, sizeof (L_ITEM));
	pl_item->class.this = (PCLASS) pl_item;
	pl_item->class.type = CLASS_L_ITEM; 
	strncpy (pl_item->class.name, name, MAX_NAME_LENGTH);
	pl_item->next = 0;
	pl_item->class.preparedelete = 
		(int (*) (PCLASS))(__l_item_preparedelete);
	pl_item->class.printattributes = 
		(int (*) (PCLASS, int))(__l_item_printattributes);
	return pl_item;
}
PPROPERTY newproperty (char *name)
{
	PPROPERTY prop = (PPROPERTY) calloc (1, sizeof (PROPERTY));
	if (prop)
	{	
		prop->l_item.class.this = (PCLASS) prop;
		prop->l_item.class.type = CLASS_PROPERTY;
		strncpy (prop->l_item.class.name, name, MAX_NAME_LENGTH - 1);
		memset (prop->value, 0, 256);
		prop->setvalue = __property_setvalue;
		prop->l_item.class.preparedelete = 
			(int (*) (PCLASS))(__property_preparedelete);
		prop->l_item.next = NULL;
		prop->l_item.class.printattributes = 
			(int (*) (PCLASS, int))(__property_printattributes);

	}
	return prop;
}

PLIST newlist (char *list_name)
{
	PLIST plist = (PLIST) calloc (1, sizeof (LIST));
	if (plist)
	{
		plist->l_item.class.this = (PCLASS) plist;
		plist->l_item.class.type = CLASS_LIST; 
		strncpy (plist->l_item.class.name, list_name, MAX_NAME_LENGTH - 1); 
		plist->l_item.next = NULL;
		plist->count = 0;
		plist->add = __list_add;
		plist->take = __list_take;
		plist->takename = __list_take_name; 
		plist->l_item.class.preparedelete = 
			(int(*)(PCLASS))(__list_preparedelete); 
		plist->l_item.class.printattributes = 
			(int(*)(PCLASS, int))(__list_printattributes);
		
	}
	return plist;
}


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


/**
 * Name : gettoken
 * Description : get the first token that match any member of token list.
 *               copy buffer read from first read until token found to buffer.
 * Input 
 *      fp        : file pointer to inspect.
 *      tokenlist : list of accpetable token, must ended with character zero.
 *      buff      : pointer to buffer where to  store bytes read from file until
 *                  token found. 
 *      max_buffsize : max bytes can be stored in buff.
 *      length_read  : length of bytes copied to buffer.
 *      
 * 	
 * Returns : index to match pointer, EOF if we already in the end of file.
 *      If no token is found then either EOF or index >= pointer to zero.  
 */
int gettoken (FILE *fp, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read)
{
	int ch  = 0;
	int i = 0;
	int j = 0;
	*length_read = 0;
	while ((ch = fgetc (fp)) != EOF)
	{
		i = 0;
		while (tokenlist[i] !=0 )
		{
			if (tokenlist[i] == (char) ch)
			{
				*length_read = j;	
				buff[j] = 0;
				return i;
			}
			i++;
		} 
		if ( j < max_buffsize )	
			buff[j++] = (int) ch; 
	}
	return -1;
}
/*
 * Name : expectnexttoken  
 * Description : Wait for a specified tokens.
 * INPUT 
 *	fp	  : file to process
 *	tokenlist : list of available token
 *	expected_token: next token that expected to present	
 * OUTPUT
 *	buff	  : buffer to store chars until next token found
 *	*bufflen  : number of bytes stored in buffer
 * RETURN VALUE
 * 	-1	: fails 
 *	others	: token index 
 *
 */
int expectnexttoken (FILE *fp, char *tokenlist, char *buff, 
	int max_buffsize, int *readlen,  char *expected_token)
{
	int i = 0;
	int token = gettoken (fp, tokenlist, buff, max_buffsize, readlen);
	while (expected_token[i] != 0)
	{
		if (tokenlist[token] == expected_token[i])
			return token;
		i ++;
	}
	return -1;
	
}

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
			fprintf (stdout, "token idx=%d val=%c", 
				token, tokenlist[token]); 
			if (length > 0) 
				fprintf (stdout, "buff = %s", buffer);
			fprintf (stdout, "\n"); 
			//getc(stdin);
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
	if (length > 0) 
		fprintf (stdout, "buff = %s", buffer);
	fprintf (stdout, "\n"); 
	return NULL;
}


int option ()
{
	fprintf (stdout, "type:\n"
			"get item_name - to set active item\n"
			"print - print properties of active item\n"
			"q - quit application\n"); 
	return 0;
}
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
typedef struct stack_ptr
{
	int top;
	void * c[100];
	int (* init)(struct stack_ptr *);
	int (* push) (struct stack_ptr *, void *);
	void * (* pop) (struct stack_ptr *);
}STACK_PTR, *PSTACK_PTR;

int __stack_ptr_init (PSTACK_PTR p)
{
	p->top  = -1;
	memset (p->c, 0, 100);
	return 0;
}
int __stack_ptr_push (PSTACK_PTR p, void * v)
{
	if (p->top >= 100)
		return -1;
	p->top++; 
	p->c[p->top] = v;
	return 0;

}
void * __stack_ptr_pop (PSTACK_PTR p)
{
	void *ret;
	if (p->top < 0)
		return NULL;
	ret = p->c[p->top--];
	return ret;
}

PSTACK_PTR newstackptr ()
{
	PSTACK_PTR p = (PSTACK_PTR) calloc (1, sizeof (STACK_PTR));
	p->top = -1;
	p->init = __stack_ptr_init;
	p->push = __stack_ptr_push;
	p->pop = __stack_ptr_pop;
	return p;	
}

int userparse (FILE *fp, char *tokenlist, PLIST *proot)
{
	char buffer [256];

	int token = -1;
	int prevtoken = -1;
	int length = 0;
	int state = 0; 
	PSTACK_PTR stack = newstackptr();

#define STATE_GET 1
#define STATE_PRINT 2
#define STATE_OTHER 0

	option();
	PLIST lcurr = 0; 
	PLIST root = *proot;
	PL_ITEM pl_icurr  = &root->l_item;
	token = gettoken (fp, tokenlist, buffer, 256,&length);
	state = STATE_OTHER;
	while (token !=-1)
	{
		switch (tokenlist[token])
		{
		case '\n':
			trimstring (buffer);
			if (!strlen(buffer))
			{
				;	
			}
			else if (!strcmp (buffer, "q"))
			{
				fprintf (fp, "danke schon...");
				return 0; 
			}
			else if (!strncmp (buffer, "print", 5))
			{
				state = STATE_PRINT;
				if (pl_icurr)
				{
					pl_icurr->class.printattributes(&pl_icurr->class, 0); 
				} 
				else
				{
					fprintf (stdout ,"empty\n"); 
				}
				state = STATE_OTHER; 
			}
			else if (!strncmp (buffer, "delete", 6))
			{
				state = STATE_OTHER;
				if (!pl_icurr)
				{
					fprintf (stdout ,"empty\n");
				}
				else
				{
					pl_icurr->class.preparedelete(&pl_icurr->class);
					free (pl_icurr);
					pl_icurr = stack->pop (stack);
				}
			}
			else
			{
				switch (state)
				{
				case STATE_GET:
					trimstring (buffer);
					if (!strlen(buffer))
						break;
					if (!pl_icurr)
					{
						fprintf (stdout ,"empty\n"); 
					}
					else if (pl_icurr->class.type == CLASS_LIST)
					{
						lcurr = (PLIST) pl_icurr;
						stack->push (stack, pl_icurr);
						pl_icurr = lcurr->takename (lcurr, buffer);
						if  (!pl_icurr )
						{
							fprintf (stdout, "not found\n");
							pl_icurr = stack->pop (stack); 
						}

					}
					state = STATE_OTHER;
					break;
				default :
					break;
				}
			}
			prevtoken = token; 
			token = gettoken (fp, tokenlist, buffer, 256,&length);
			break;
		case ' ':
			if (token >= 0)
			{ 
				if (strncmp (buffer, "get", 3))
				{ 
					state = STATE_OTHER;
					fprintf (stderr,"[ER] unknow option\n");
					token = gettoken (fp, tokenlist, buffer, 256,&length); 
					//cleanup();
					break;
				}
				else
				{ 
					state = STATE_GET; 
					prevtoken = token;
					token = gettoken (fp, tokenlist, buffer, 256,&length); 
					break;
				}


			} 
			break;	
		} 
	}
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

typedef struct miniparser
{
	FILE *input;
	char *tokenlist;
	int (*parse) (struct miniparser *, PLIST *); 
}MINIPARSER, *PMINIPARSER;
static int infile_parse (PMINIPARSER parser, PLIST *plist)
{
	preprocessfile (parser->input, parser->tokenlist, plist);
	return 0;
}

static int stdin_parse (PMINIPARSER parser, PLIST *plist)
{
	userparse (parser->input, parser->tokenlist, plist);
	return 0;
}

PMINIPARSER newminiparser (FILE *in, char *tokenlist, int (*f)(PMINIPARSER, PLIST *))
{
	PMINIPARSER p = (PMINIPARSER) calloc (1, sizeof(MINIPARSER));
	p->input = in;
	p->tokenlist = tokenlist;
	p->parse = f;	
	return p;
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
	root->l_item.class.printattributes(&root->l_item.class, 0);
	
	pstdio = newminiparser (stdin, " \n", stdin_parse);	
	pstdio->parse (pstdio,&root);
	/*
	root->l_item.class.preparedelete(&root->l_item.class);
	*/
	free(root);
	exit (0);
}
