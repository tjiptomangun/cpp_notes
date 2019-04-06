/* parserclass.c 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c -Wall -ggdb3 -o xparsecfg
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parserclass.h"

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
}


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
 
PMINIPARSER newminiparser (FILE *in, char *tokenlist, int (*f)(PMINIPARSER, PLIST *))
{
	PMINIPARSER p = (PMINIPARSER) calloc (1, sizeof(MINIPARSER));
	p->input = in;
	p->tokenlist = tokenlist;
	p->parse = f;	
	return p;
}
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

