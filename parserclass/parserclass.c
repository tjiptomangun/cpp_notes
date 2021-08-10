/**
 * author : henky <hanky.acbb@telogic.com.sg>
 */

/* parserclass.c 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c -Wall -ggdb3 -o xparsecfg
 * here we try object oriented paradigm to our structures.
 * to do this, our structures shall contain pointers to 
 * functions that each of them eligible to call.
 * the implementation of the function, namely kernel function
 * always begins with __ (double under score) followed by
 * class name that it is the implemented to. 
 * if this kernel function calls another functions either within
 * its own class or of another class, then it is better to use 
 * class implementation of the function rather than kernel function. 
 * collection class : tree and list
 * take** will detach
 * get** will not detach item
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parserclass.h"
#include <assert.h>

size_t strlen(const char *s);
void print_identation (int ident)
{
	int i;
	for (i = 0; i < ident; i++)
		fprintf (stdout, "\t"); 
} 

char *trimstring (char *instr)
{
	if (instr) {
		size_t len = strlen(instr);
		int first = 0;
		int last = len;
		int k = 0;
		int i = 0;
		if (!len)
			return instr;

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
	return instr;
}


static int __class_delete (void *arg1)
{
	PCLASS p = (PCLASS) arg1;
	memset (p->name, 0, MAX_NAME_LENGTH);
	p->this = NULL;
	free (p); 
	p = NULL;
	return 0;
}
static int __class_printattributes(void* arg1, int ident)
{
	PCLASS p = (PCLASS) arg1;
	print_identation (ident);
	if (p->name[0])
		fprintf (stdout, "class.name : %s", p->name);
	else
		fprintf (stdout, "class.name : ");
	fprintf (stdout, "\n");
	return 0;
}

void __class_ctor (PCLASS pclass,char *name)
{ 
	pclass->this = pclass;
	pclass->type = CLASS_CLASS;
	strncpy (pclass->name, name, MAX_NAME_LENGTH); 
	pclass->printattributes = (int (*) (void*, int))__class_printattributes; 
	pclass->delete = __class_delete; 
}

PCLASS newclass (char *name)
{
	PCLASS pclass = (PCLASS) calloc (1, sizeof (CLASS));
	if (pclass)
		__class_ctor (pclass, name);
	return pclass; 
} 

static int __l_item_printattributes(void *arg1, int ident)
{
	PL_ITEM p = (PL_ITEM) arg1;
	return __class_printattributes(&p->class, ident); 
}
/**
 * Name		: __l_item_delete 
 * Description	: override __class_delete for ITEM structure
 * Returns	
 *		0 on success
 */
static int __l_item_delete(void *arg1)
{
	PL_ITEM p = (PL_ITEM) arg1;
	p->next = NULL;
	return __class_delete(& p->class);
}

void __l_item_ctor (PL_ITEM pl_item, char *name)
{
	__class_ctor (&pl_item->class, name); 
	pl_item->class.type = CLASS_L_ITEM; 
	pl_item->next = 0;
	pl_item->class.printattributes = 
		__l_item_printattributes;
	pl_item->class.delete = 
		__l_item_delete;
}

PL_ITEM newl_item (char *name)
{
	PL_ITEM pl_item = (PL_ITEM) calloc (1, sizeof (L_ITEM));
	if (pl_item)
		__l_item_ctor (pl_item, name);
	return pl_item;
}


static int __list_printattributes(void *arg1, int ident)
{
	PLIST p = (PLIST) arg1;
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
 * NAME		: __list_addproperty
 * DESCRIPTION	: add new property
 * INPUT
 *		- plist , this pointer
 *		- name , name of this property
 *		- value, value of this property
 **/
static int  __list_addproperty (PLIST plist, char *name, char *value)
{
	PPROPERTY add = newproperty2 (name, value);
	if (!add)
		return -1; 
	if (plist->head == NULL)
	{
		plist->head = &add->l_item;
		plist->tail = plist->head; 
		plist->currptr = plist->head;
		plist->count++; 
	}
	else
	{
		plist->tail->next = &add->l_item;
		plist->tail = &add->l_item; 
		plist->count++;
	}
	plist->tail->next = NULL;
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
		plist->currptr = plist->head;
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
 * NAME		: __list_detach
 * DESCRIPTION	: detach a specified list item
 * RETURNS	: 0 if success
 *		  pointer to detached if it is not in the list
 *
 */
static PL_ITEM  __list_detach (PLIST plist, PL_ITEM detached)
{
	PL_ITEM curr = plist->head;
	while (curr)
	{
		if (detached == curr){
			if (detached == plist->currptr)
				plist->currptr = plist->currptr->next;
			return NULL;
		}
		curr = curr->next;
	} 
	return detached;
}

/**
 * NAME		: __list_take
 * DESCRIPTION	: take head
 *                returned head detached
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
 * NAME 	: __list_delete
 * DESCRIPTION	: clean up the list and delete object
 */
static int __list_delete(void *arg1)
{
	PLIST plist = (PLIST) arg1;
	PL_ITEM curr;
	while (( curr = __list_take(plist)))
	{
		curr->class.delete (&curr->class);
		plist->count --;
	}
	return __class_delete (&plist->l_item.class);
}

/**
 * NAME		: __list_takename
 * DESCRIPTION	: take member element of name xx
 *                returned item is detached
 */
static PL_ITEM __list_takename(PLIST plist, char* name)
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

/**
 * NAME		: __list_getname
 * DESCRIPTION	: get member element of name xx
 *                returned item is not detached from list
 */
static PL_ITEM __list_getname(PLIST plist, char* name)
{
	PL_ITEM curr = plist->head; 
	while (curr)
	{ 
		if (!strncmp (name, curr->class.name, MAX_NAME_LENGTH))
		{
			return curr;
		}
		curr = curr->next;
	} 
	return NULL;
}

/**
 * NAME		: __list_getfirstchild
 * DESCRIPTION	: returns the first child of this item
 *		  move the curr to next item of the first child
 */
static PL_ITEM __list_getfirstchild (PLIST plist)
{
	plist->currptr = plist->head;
	if (plist->currptr) 
		plist->currptr = plist->currptr->next;
	return plist->head;
}

/**
 * NAME		: __list_getnextchild
 * DESCRIPTION	: returns the next  child of this list.
 *		  move the curr to next item of curr item.
 */
static PL_ITEM __list_getnextchild (PLIST plist)
{
	PL_ITEM ret = plist->currptr;
	if (plist->currptr)
		plist->currptr = plist->currptr->next;
	return ret;	
}

void __list_ctor (PLIST plist, char *list_name)
{ 
	__l_item_ctor (&plist->l_item, list_name);
	plist->l_item.class.type = CLASS_LIST; 
	plist->currptr = NULL; 
	plist->l_item.next = NULL;
	plist->count = 0;
	plist->add = __list_add;
	plist->take = __list_take;
	plist->takename = __list_takename; 
	plist->getname = __list_getname; 
	plist->getfirstchild = __list_getfirstchild; 
	plist->getnextchild= __list_getnextchild; 
	plist->l_item.class.printattributes = __list_printattributes;
	plist->l_item.class.delete= __list_delete; 
	plist->detach = __list_detach; 
	plist->addproperty = __list_addproperty;
}

void list_resetlist (PLIST plist, char *list_name)
{ 
    __list_ctor(plist, list_name);
}


PLIST newlist (char *list_name)
{
	PLIST plist = (PLIST) calloc (1, sizeof (LIST));
	list_resetlist(plist, list_name);
	return plist;
}


static int __property_delete(void *arg1) 
{
	PPROPERTY p = (PPROPERTY) arg1;
	memset (p->value, 0, sizeof(p->value));
	__class_delete (&p->l_item.class);
	return 0;
}

static int __property_setvalue (PPROPERTY p, char *value)
{
	strncpy (p->value, value, sizeof(p->value));
	return 0;
}

static int __property_getvalue (PPROPERTY p, char *value, size_t max_len)
{
	strncpy (value, p->value, max_len);
	return 0;
}

static char* __property_getvalue_ptr (PPROPERTY p) {
	return p->value;
}

static int __property_printattributes(void *arg1, int ident)
{
	PPROPERTY p = (PPROPERTY) arg1;
	__l_item_printattributes(&p->l_item, ident);	
	print_identation (ident);
	fprintf (stdout, "property.value : %s", p->value);
	fprintf (stdout, "\n");
	return 0;
}

/**
 * NAME		: __property_ctor 
 * DESCRIPTION	: Constructor for propery
 * INPUT	  
 *		- prop, self pointer
 *		- name, object name
 */
void __property_ctor (PPROPERTY prop, char *name)
{
	__l_item_ctor (&prop->l_item, name);
	prop->l_item.class.this = (PCLASS) prop;
	prop->l_item.class.type = CLASS_PROPERTY;
	strncpy (prop->l_item.class.name, name, MAX_NAME_LENGTH - 1);
	memset (prop->value, 0, 256);
	prop->setvalue = __property_setvalue;
	prop->getvalue_ptr = __property_getvalue_ptr;
	prop->getvalue= __property_getvalue;
	prop->l_item.next = NULL;
	prop->l_item.class.printattributes = __property_printattributes;
	prop->l_item.class.delete = __property_delete;
}

/**
 * NAME		: __property_ctor2 
 * DESCRIPTION	: Constructor for propery
 * INPUT	  
 *		- prop, self pointer
 *		- name, object name
 *		- value, object value
 */
void __property_ctor2 (PPROPERTY prop, char *name, char *value)
{ 
	__l_item_ctor (&prop->l_item, name);
	prop->l_item.class.this = (PCLASS) prop;
	prop->l_item.class.type = CLASS_PROPERTY;
	strncpy (prop->l_item.class.name, name, MAX_NAME_LENGTH - 1);
	memset (prop->value, 0, 256);
	prop->setvalue = __property_setvalue;
	prop->getvalue_ptr = __property_getvalue_ptr;
	prop->getvalue= __property_getvalue;
	prop->l_item.next = NULL;
	__property_setvalue (prop, value);
	prop->l_item.class.printattributes = __property_printattributes;
	prop->l_item.class.delete = __property_delete;
}

PPROPERTY newproperty (char *name)
{
	PPROPERTY prop = (PPROPERTY) calloc (1, sizeof (PROPERTY));
	if (prop)
	{	
		__property_ctor (prop, name);
	}
	return prop;
}

PPROPERTY newproperty2 (char *name, char *value)
{
	
	PPROPERTY prop = (PPROPERTY) calloc (1, sizeof (PROPERTY));
	if (prop)
	{	
		__property_ctor2 (prop, name, value);
	}
	return prop;
}

int __stack_ptr_init (PSTACK_PTR p)
{
	p->top  = -1;
	memset (p->c, 0, MAX_STACKPTR * sizeof(void *));
	return 0;
}

int __stack_ptr_push (PSTACK_PTR p, void * v)
{
	if (p->top >= MAX_STACKPTR)
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

/**
 * Name		: __stack_ptr_cleanup
 * Description	: release all pointers in this stack
 **/
int  __stack_ptr_cleanup (PSTACK_PTR p)
{
	void * ptr;
	while (p->top >= 0)
	{
		ptr = __stack_ptr_pop (p);
		if (!ptr)
			continue;
		free (ptr);
	}
	return __stack_ptr_init (p);

} 

int __stack_ptr_isempty (PSTACK_PTR p)
{
	int ret = 0;
	ret = (p->top < 0)? 1: 0;
	return ret;
}

PSTACK_PTR newstackptr ()
{
	PSTACK_PTR p = (PSTACK_PTR) calloc (1, sizeof (STACK_PTR));
	if(p) {
		p->top = -1;
		p->init = __stack_ptr_init;
		p->push = __stack_ptr_push;
		p->pop = __stack_ptr_pop;
		p->cleanup = __stack_ptr_cleanup;
		p->is_empty = __stack_ptr_isempty;
	}
	return p;	
}
 
PMINIPARSER newminiparser (FILE *in, char *tokenlist, int (*f)(PMINIPARSER, PLIST *))
{
	PMINIPARSER p = (PMINIPARSER) calloc (1, sizeof (MINIPARSER));
	p->input = in;
	p->tokenlist = tokenlist;
	p->parse = f;	
	return p;
}
/*
 * NAME		: newcharparser.
 * DESCRIPTION	: new char parser
 * 		  Do not delete tokenlist and in until parsing done.
 */ 

PCHARPARSER newcharparser (char *in, int inlen, char *tokenlist, 
				int (*f)(PCHARPARSER, PLIST *))
{
	PCHARPARSER p = (PCHARPARSER) calloc (1, sizeof (CHARPARSER)); 
	p->input = in;
	p->currpos = -1;
	p->charlen = inlen;
	p->tokenlist = tokenlist;
	p->parse  = f;	
	return p;
}
/**
 * Name : stream_gettoken
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
int stream_gettoken (FILE *fp, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read)
{
	int ch  = 0;
	int i = 0;
	int j = 0;
	*length_read = 0;
	while (fp && (ch = fgetc (fp)) != EOF)
	{
		i = 0;
		while (tokenlist[i] !=0 && j < max_buffsize)
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
 * Name : stream_expectnexttoken  
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
int stream_expectnexttoken (FILE *fp, char *tokenlist, char *buff, 
	int max_buffsize, int *readlen,  char *expected_token)
{
	int i = 0;
	int token = stream_gettoken (fp, tokenlist, buff, max_buffsize, readlen);
	while (expected_token[i] != 0)
	{
		if (tokenlist[token] == expected_token[i])
			return token;
		i ++;
	}
	return -1;
	
}

/**
 * Name : string_gettoken
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
int string_gettoken (char *input, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read)
{
	int ch  = 0;
	int i = 0;
	int j = 0;
	int ndx = 0;
	*length_read = 0;
	while ((ch = input[ndx++]) != 0)
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
		{
			buff[j++] = (int) ch; 
			*length_read = j;
		}
	}
	return -1;
}


/*
 * Name : string_expectnexttoken  
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
int string_expectnexttoken (char *input, char *tokenlist, char *buff, 
	int max_buffsize, int *readlen,  char *expected_token)
{
	int i = 0;
	int token = string_gettoken (input, tokenlist, buff, max_buffsize, readlen);
	while (expected_token[i] != 0)
	{
		if (tokenlist[token] == expected_token[i])
			return token;
		i ++;
	}
	return -1;
	
}

/**
 * NAME		: parse_env_str
 * DESCRIPTION	: parse input string, interpret all prefixed $ string as
 *		  environment variables. Get the environment vars, replace
 *		  the prefixed $ string with env variable
 * INPUT	
 *	instring	: input string to be interpreted 
 *	newenv_max	: max length of buffer to store interpretted value
 *
 * OUTPUT
 *	newenv		: buffer to store interpretted value
 *
 * RETURNS
 *			: on return, returns length of interrpretted string
 */
#define ENVBUF_SZ 4096
int parse_env_str (char *instring, char *newenv, int newenv_max )
{
	char *pnewenv = &newenv [0];
	int token ;
	char buff [ENVBUF_SZ];
	int  readlen = 0;
	char *orig_env = 0;
	char *pvalue = instring;
 
	memset (newenv, 0, newenv_max);
	memset (buff, 0, ENVBUF_SZ);
	token = -1;

		
	token = string_gettoken (pvalue, 
		"$", buff, ENVBUF_SZ, &readlen); 
	while (token >=0)
	{
		if (readlen)
		{
			if ((newenv_max - (pnewenv - &newenv[0])) <readlen)
				break;
			memcpy (pnewenv, buff, readlen ); 
			pnewenv += readlen < ENVBUF_SZ ? readlen : ENVBUF_SZ; 
			pvalue +=readlen;
		}
		readlen = 1; /*advance 1 token*/
		pvalue += readlen;
		readlen = 0;
		memset (buff, 0, ENVBUF_SZ);

		token = string_gettoken(pvalue, "$/:", buff, ENVBUF_SZ, &readlen);
		if (!readlen)
			break;
		
		/*
		 * getenv here
		 * write to new env
		 */
		pvalue += readlen;
		orig_env = getenv(buff);
		if (orig_env)
		{
			memcpy (pnewenv, orig_env, strlen (orig_env));
			pnewenv += strlen (orig_env);
		} 
		memset (buff, 0, ENVBUF_SZ);
		readlen = 0;
		if (token == -1)
		{
			break;
		}
		if (token == 0)
		{
			continue;
		}
		else
		{ 
			token = string_gettoken (pvalue, "$", buff, ENVBUF_SZ, &readlen); 
		} 
	} 
	if (readlen)
	{
		if ((ENVBUF_SZ - (pnewenv - &newenv[0])) >readlen)
		{ 
			memcpy (pnewenv, buff, readlen ); 
			pnewenv += readlen < ENVBUF_SZ? readlen : ENVBUF_SZ; 
		}
	}
	return pnewenv - newenv; 
}

static int __treeitem_add (struct tree_item *root, struct tree_item *addedchild)
{
	if (!root->head)
	{
		root->head = addedchild;
		root->tail = addedchild;	
	}
	else
	{
		root->tail->next = addedchild;
		root->tail = addedchild;
	}
	addedchild->parent = root;
	return 0;
}

static struct tree_item * __treeitem_getparent (struct tree_item *root)
{
	return root->parent;
}

/**
 * Name		: __treeitem_getfirstchild
 * Description	: get first child of this root
 *		  currchild will point to next item of 
 *		  head. 
 */
static struct tree_item * __treeitem_getfirstchild (struct tree_item *root)
{
	struct tree_item *ret = root->head;
	if (root->head){
		root->curr = root->head;
		root->curr = (PTREE_ITEM) root->curr->next;
	}
	return ret;
}

/**
 * Name		: __treeitem_getnextchild
 * Description	: return currchild
 *		: move currchild to current currchild next item
 */
static struct tree_item * __treeitem_getnextchild (struct tree_item *root)
{
	struct tree_item *ret = root->curr;
	if (root->curr)
		root->curr = (PTREE_ITEM) root->curr->next;
	return ret;
}

/**
 * Name		: __treeitem_takechild
 * Description	: take and detach child head
 *		  next child becomes current head 
 */
static struct tree_item * __treeitem_takechild (struct tree_item *root)
{
	struct tree_item * ret = 0;
	if (root->head)
	{
		ret = root->head;
		root->head = (PTREE_ITEM) root->head->next; 
	}
	return ret;
} 

/**
 * NAME		: __treeitem_listdelete
 * DESCRIPTION	: clean up the list and delete object
 */
static int __treeitem_listdelete(void *arg1)
{
	PTREE_ITEM root = (PTREE_ITEM ) arg1;
	PL_ITEM curr;
	while (( curr = root->list.take(&root->list)))
	{
		curr->class.delete (&curr->class);
		root->list.count --;
	}
	return 0;
}

/*
 * NAME 									: __treeitem_detach
 * DESCRIPTION 						: detach specified list item
 * RETURNS								: 
 * 					NULL					: if success 
 *	 pointer to detached 	: if not found
 */
static PTREE_ITEM  __treeitem_detach(struct tree_item *root, struct tree_item *detached)
{
	PTREE_ITEM curritem = root->head;
	PTREE_ITEM prev = NULL;
	while (curritem)
	{
		if (detached == curritem) {
			if (prev){
				prev->next = curritem->next;
			}
			else {
				root->head = curritem->next;
			}
			return NULL;
		}
		prev = curritem;
		curritem = curritem->next;
	}
	return detached;
}

/**
 * NAME		: __treeitem_delete
 * DESCRIPTION	: delete a treeitem along with its child
 */
static void  __treeitem_delete (struct tree_item *root)
{
	struct tree_item* child = __treeitem_takechild(root);
	while (child)
	{ 
		__treeitem_delete (child); 
		child = __treeitem_takechild(root);
	}
	__treeitem_listdelete (root);
	__class_delete((PCLASS)root);	
}

/**
 * NAME		:__treeitem_takename
 * DESCRIPTION	: take member element of name xx
 *		  returned item is detached
 */
static PTREE_ITEM __treeitem_takename (PTREE_ITEM root, char *name)
{
	PTREE_ITEM curr = root->head;
	PTREE_ITEM prev = NULL;

	while (curr)
	{
		if (!strncmp (name, curr->list.l_item.class.name, MAX_NAME_LENGTH))
		{
			if (prev)
			{
				prev->next = curr->next;
				curr->next = NULL;
			}
			else
			{
				root->head = curr->next;
				curr->next = NULL;
			}
			return curr;
		}
		prev = curr;
		curr = curr->next;
	}
	return NULL;
}

/**
 * NAME		: __treeitem_getname
 * Description	: get member element of name xx
 *		  returned item is not detached from list
 */
static PTREE_ITEM __treeitem_getname (PTREE_ITEM root, char *name)
{
	PTREE_ITEM curr = root->head;

	while (curr)
	{
		if (!strncmp (name, curr->list.l_item.class.name, MAX_NAME_LENGTH))
		{
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

static int __treeitem_printattributes(void *arg1, int ident){
	PTREE_ITEM root = (PTREE_ITEM) arg1;
	__list_printattributes(&root->list, ident + 1);
	PTREE_ITEM curr = root->head;
	while(curr){
		__treeitem_printattributes(curr, ident + 1);
		curr = curr->next;
	}
	return 0;
}


void __treeitem_ctor (PTREE_ITEM  new, PTREE_ITEM parent, char *name)
{
	__list_ctor (&new->list, name);
	new->head = new->tail = new->curr = new->next = 0;
	new->parent = parent;
	list_resetlist (&new->list, name);
	new->list.l_item.class.delete = __treeitem_listdelete;
	new->add = __treeitem_add;
	new->getparent = __treeitem_getparent;
	new->getfirstchild = __treeitem_getfirstchild;
	new->getnextchild = __treeitem_getnextchild;
	new->takechild = __treeitem_takechild;
	new->takename = __treeitem_takename;
	new->getname = __treeitem_getname;
	new->detach = __treeitem_detach;
	new->delete = __treeitem_delete;
	new->list.l_item.class.printattributes = __treeitem_printattributes;
	
}
/*
 * NAME		: newtreeitem
 * Description	: alloc memory for treeitem
 *		  assign values and pointers
 */

struct tree_item * newtreeitem(struct tree_item *parent, char *name)
{
	PTREE_ITEM new = (PTREE_ITEM)calloc (1, sizeof (TREE_ITEM));
	if (new)
			__treeitem_ctor(new, parent, name);
	
	return new;
}

/**
 * Name		: __circularitem_delete
 * Description	: delete the supplied circular item
 * 		  set data to null and free the data
 **/
int __circularitem_delete(void *arg1)
{
	PCIRCULARITEM item = (PCIRCULARITEM) arg1;
	item->next = item->prev = NULL; 
	if (item->data)
	{
		memset (item->data, 0, item->datasize); 
		free (item->data);
	}
	memset (item, 0 , sizeof (CIRCULARITEM));
	free (item);
	return 0;
}

int  __circularitem_printattributes (void * arg1, int ident)
{
	PCIRCULARITEM citem = (PCIRCULARITEM) arg1;
	__class_printattributes (&citem->class, ident);
	print_identation (ident);
	fprintf (stdout, "prev : %p\n",  citem->prev);
	print_identation (ident);
	fprintf (stdout, "next : %p\n",  citem->next); 
	return 0;
	
} 
void __circularlist_add (PCIRCULARLIST clist, PCIRCULARITEM citem)
{
	if (!clist->current)
	{
		clist->current = citem;
		citem->prev = citem->next = citem; 
	}
	else
	{
		if (clist->current->prev)
		{
			clist->current->prev->next = citem;
			citem->next = clist->current;
			citem->prev = clist->current->prev;
			clist->current->prev = citem;
		}
	}
	clist->count ++;
}
int __circularlist_printattributes (void *arg1, int ident)
{
	PCIRCULARLIST clist = (PCIRCULARLIST) arg1;
	PCIRCULARITEM citem;
	int k = clist->count;
	__class_printattributes (&clist->class, ident);
	citem = clist->current;
	while (k)
	{
		citem->class.printattributes(&citem->class, ident+1);
		citem = citem->next;	
		k--;
	}
	return 0;	
		
}
/**
 * Name		: __circularitem_take 
 * Description	: take current item from list 
 *		  taken item is detached from list
 */
PCIRCULARITEM __circularlist_take (PCIRCULARLIST clist)
{
	PCIRCULARITEM citem = clist->current;
	if (citem)
	{
		if (citem->prev)
		{
			citem->prev->next = citem->next; 
		}
		if (citem->next)
		{
			citem->next->prev = citem->prev;
		}
		if (citem->next == citem)
		{ 
			clist->current = NULL;		
		}
		else
		{
			clist->current = citem->next;
		}
		citem->next = citem->prev = NULL;
		clist->count --; 
		
	}
	return citem;
	
}

/**
 * Name		: __circularitem_takename 
 * Description	: take an item of name from list
 * Returns 	: the item if there is
 *		  NULL others 
 *
 */
PCIRCULARITEM  __circularlist_takename (PCIRCULARLIST clist, char *name)
{
	PCIRCULARITEM citem = clist->current;
	int i = 0;
	for (i = 0; i < clist->count ; i++)
	{
		if (!strcmp (citem->class.name, name) )
		{
			if (citem->prev)
			{
				citem->prev->next = citem->next;
			}
			if (citem->next)
			{
				citem->next->prev = citem->prev;
			}
			if (citem == clist->current)
			{
				if (citem->next == citem)
					clist->current = NULL;
				else
					clist->current = citem->next;
			}
			return citem;
		}
		citem = citem->next;

	}
	return NULL;
	
}

/*
 * Name		: __circularlist_remove
 * Description	: remove particular item from the list.
 * Returns	  NULL if item is of this list
 *		  Others if item is not of this list
 */
PCIRCULARITEM  __circularlist_remove (PCIRCULARLIST clist, PCIRCULARITEM citem)
{
	PCIRCULARITEM temp;
	temp = clist->current;
	if (!temp)
		return citem;
	do
	{
		if (citem == temp)
		{
			if (citem->prev)
				citem->prev->next  = citem->next;
			if (citem->next)
				citem->next->prev  = citem->prev;
			if (citem->next == citem)
				clist->current = NULL;
			else
				clist->current = citem->next;
			clist->count --;
			citem->class.delete ((PCLASS)citem);
			return NULL;
		}
		temp = temp->next;
	}while (temp != clist->current);
	return citem;
}
/**
 * Name		: __circularlist_delete
 * Description	: override class delete
 */
int __circularlist_delete (void *arg1)
{
	PCIRCULARLIST clist = (PCIRCULARLIST) arg1;
	while (clist->current)
	{
		__circularlist_remove (clist, clist->current);
	}
	clist->add = NULL;
	clist->remove = NULL;
	memset (clist, 0, sizeof (CIRCULARLIST));
	free (clist); 
	return 0;
} 

void __circularitem_ctor (PCIRCULARITEM pciri, char *name, void *data, int size)
{

	pciri->class.this = (PCLASS) pciri;
	pciri->class.type = CLASS_CIRCULARITEM;
	strncpy (pciri->class.name, name, MAX_NAME_LENGTH - 1); 
	pciri->next = pciri->prev = NULL;
	pciri->class.delete =__circularitem_delete;
	pciri->class.printattributes = 
		(int (*) (void*, int))(__circularitem_printattributes);
	pciri->datasize = size;
	pciri->data = data; 	;
}

/**
 * Name		: newcircularitem
 * Description	: new circularitem
 */

PCIRCULARITEM newcircularitem (char *name, void *data, int size)
{
	PCIRCULARITEM pciri = (PCIRCULARITEM) calloc (1, sizeof (CIRCULARITEM));
	if (pciri)
	{
		__circularitem_ctor (pciri, name, data, size);
	}
	return pciri;
}
void __circularlist_ctor (PCIRCULARLIST pcirlist, char *name)
{
	__class_ctor (&pcirlist->class, name); 
	pcirlist->class.type = CLASS_CIRCULARLIST; 
	pcirlist->add = (void (*) (PCIRCULARLIST, PCIRCULARITEM)) 
			 (__circularlist_add);
	pcirlist->remove = __circularlist_remove;
	pcirlist->take = (PCIRCULARITEM (*) (PCIRCULARLIST))
			 (__circularlist_take);
	pcirlist->takename = (PCIRCULARITEM (*) (PCIRCULARLIST, char *))
			 (__circularlist_takename);
	pcirlist->class.delete = __circularlist_delete;
	pcirlist->class.printattributes = __circularlist_printattributes;

}
/**
 * Name		: newcircularlist
 * Description	: new circularlist
 *
 */
PCIRCULARLIST newcircularlist (char *name)
{
	PCIRCULARLIST pcirlist = (PCIRCULARLIST) calloc (1, sizeof (CIRCULARLIST));
	if (pcirlist)
	{
		__circularlist_ctor (pcirlist, name);
	}
	return pcirlist;
}

static int __primclass_delete (void *arg1)
{
	PPRIMCLASS p = (PPRIMCLASS) arg1;
	p->this = NULL;
	free (p); 
	p = NULL;
	return 0;
}

PPRIMCLASS __newprimclass ()
{
	PPRIMCLASS pclass = (PPRIMCLASS) calloc (1, sizeof (PRIMCLASS));
	if (pclass) {
		pclass->this = pclass;
		pclass->type = PRIMCLASS_PRIMCLASS;
		pclass->delete = __primclass_delete; 
	}
	return pclass;
}

void* __priml_item_set_data (void *arg1, void *data) {
	PPRIML_ITEM p = (PPRIML_ITEM) arg1;
	if (!data){
		return NULL;
	}
	else {
		p->data = data;
		return (void*) p;
	}
}

static int __priml_item_delete(void *arg1) {
	PPRIML_ITEM node = (PPRIML_ITEM) arg1;
	if (!node) {
		return 1;
	}
	else {
		if(node->data) {
			if (node->data_remove_fn) {
				node->data_remove_fn(node->data);
			}
			else {
				free(node->data);
			}
			node->data = NULL;
		}
		node->next = NULL;
		node->set_data = NULL;
		node->delete = NULL;
		free(node); 
	}
	return 0;
} 

static void * __priml_item_get_data (void *arg1) {
		PPRIML_ITEM p = (PPRIML_ITEM) arg1;
		if(p){
			return p->data;
		}
		else {
			return NULL;
		}
}

void __priml_item_set_data_remove_fn (PPRIML_ITEM item, int (*fn) (void *)) {
	item->data_remove_fn = fn;
}
static PPRIML_ITEM __priml_item_ctor(PPRIML_ITEM ppriml_item) {
	if(ppriml_item) {
		ppriml_item->primclass.this = (PPRIMCLASS) ppriml_item;
		ppriml_item->primclass.type = PRIMCLASS_PRIMLITEM; 
		ppriml_item->next = NULL;
		ppriml_item->data = NULL;
		ppriml_item->primclass.delete = __priml_item_delete;
		ppriml_item->set_data = __priml_item_set_data;
		ppriml_item->delete = __priml_item_delete;
		ppriml_item->get_data = __priml_item_get_data;
		ppriml_item->set_data_remove_fn = __priml_item_set_data_remove_fn;
	}
	return ppriml_item;
	
}
PPRIML_ITEM newpriml_item ()
{
	PPRIML_ITEM ppriml_item = (PPRIML_ITEM) calloc (1, sizeof (PRIML_ITEM));
	if (ppriml_item) {
		__priml_item_ctor(ppriml_item);
	}
	return ppriml_item;
}

/**
 * NAME		: __primlist_detach
 * DESCRIPTION	: detach a specified list item
 * RETURNS	: 0 if success
 *		  pointer to detached if it is not in the list
 *
 */
static PPRIML_ITEM  __primlist_detach (PPRIMLIST plist, PPRIML_ITEM detached)
{
	PPRIML_ITEM curr = plist->head;
	while (curr)
	{
		if (detached == curr)
			return NULL;
		curr = curr->next;
	} 
	return detached;
}

/**
 * NAME		: __primlist_take
 * DESCRIPTION	: take head
 *                returned head detached
 */
static PPRIML_ITEM __primlist_take (PPRIMLIST plist)
{
	PPRIML_ITEM ret;
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
 * NAME 	: __primlist_delete
 * DESCRIPTION	: clean up the list and delete object
 */
static int __primlist_delete(void *arg1)
{
	PPRIMLIST plist = (PPRIMLIST) arg1;
	PPRIML_ITEM curr;
	while (( curr = __primlist_take(plist)))
	{
		curr->primclass.delete (&curr->primclass);
		plist->count --;
	}
	__primclass_delete (&plist->priml_item.primclass);
	return 0;
}

/**
 * NAME		: __primlist_getfirstchild
 * DESCRIPTION	: returns the first child of this item
 *		  move the curr to next item of the first child
 */
static PPRIML_ITEM __primlist_getfirstchild (PPRIMLIST plist)
{
	plist->currptr = plist->head;
	if (plist->currptr) 
		plist->currptr = plist->currptr->next;
	return plist->head;
}

/**
 * NAME		: __primlist_getnextchild
 * DESCRIPTION	: returns the next  child of this list.
 *		  move the curr to next item of curr item.
 */
static PPRIML_ITEM __primlist_getnextchild (PPRIMLIST plist)
{
	PPRIML_ITEM ret = plist->currptr;
	if (plist->currptr)
		plist->currptr = plist->currptr->next;
	return ret;
}

static int __primlist_collect(struct primlist * in, int  (*filter_fn) (void *), void (*collect_fn)(void *, void *), void *collected) {
	int counter = 0;
	PPRIML_ITEM curr =  __primlist_getfirstchild(in);
	void *data = NULL;
	while(curr) {
		if ((data = curr->get_data(curr) ) &&  filter_fn(data)) {
			collect_fn(data, collected);
			counter ++;
		}
		curr  = __primlist_getnextchild (in);
	}
	return counter;
}

static PPRIML_ITEM __primlist_find_one (PPRIMLIST node, void *to_find, int (*cmp) (void *, void *)) {
	PPRIML_ITEM curr = node->head;
	void *curr_data = NULL;
	int res = -1;
	if(to_find) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(to_find, curr_data)) > 0 ) {
			curr = curr->next;
		}
		if (res) {
			return NULL;
		}
		else {
			return curr;
		}
	}
	return NULL;
}

static PPRIMLIST __primlist_add_one (PPRIMLIST node, PPRIML_ITEM added, int (*cmp) (void *, void *)) {
	PPRIML_ITEM curr = node->head;
	PPRIML_ITEM prev= NULL;
	void *curr_data = NULL;
	void *added_data = NULL;
	PPRIML_ITEM tmp = NULL; 
	int res = -1;
	if (added && (added_data = added->get_data(added))) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(added_data, curr_data)) > 0 ) {
			prev= curr;
			curr = curr->next;
		}
		//match with the first item
		if (!prev && res == 0) {
			added->next = node->head->next;
			tmp = node->head;
			node->head = added;
			if (node->tail == tmp) {
				node->tail = added;
			}
			tmp->delete(tmp);
		}
		//smaller then first item
		else if (!prev && res < 0) {
			added->next = node->head;
			node->head = added;
			if (!node->tail) {
				node->tail = added;
			}
			node->count++;
		}
		//add to tail
		else if (res > 0) {
			prev->next = added;
			node->tail = added;
			node->count++;
		}
		else if (res == 0) {
			tmp = curr;
			added->next = curr->next;
			if(node->tail == curr) {
				node->tail = added;
			}
			prev->next = added;
			tmp->delete(tmp);
		}
		else if (res < 0) {
			added->next = curr;
			prev->next = added;
			node->count++;
		}
		
	}
	else {
		return NULL;
	}
	
	node->currptr = node->head;
	return node;
	
}

static PPRIMLIST __primlist_remove_one (PPRIMLIST node, void *deleted, int (*cmp) (void *, void *)) {
	PPRIML_ITEM curr = node->head;
	PPRIML_ITEM prev= NULL;
	void *curr_data = NULL;
	PPRIML_ITEM tmp = NULL; 
	int res = -1;
	if (deleted) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) > 0 ) {
			prev= curr;
			curr = curr->next;
		}
		if (res) {
			return NULL;
		}
		//match with the first item
		else if (!prev) {
			tmp = node->head;
			if (node->head == node->tail) {
				node->tail = node->head->next;
			}
			node->head = node->head->next;
			tmp->delete(tmp);
			node->count--;
		}
		else {
			if (curr == node->tail)  {
				node->tail = node->tail->next;
			}
			tmp = curr;
			prev->next = curr->next;
			tmp->delete(tmp);
			node->count--;
		}
	}
	else {
		return NULL;
	}
	
	node->currptr = node->head;
	return node;
	
}

static PPRIMLIST __primlist_add_common (PPRIMLIST node, PPRIML_ITEM added, int (*cmp) (void *, void *)) {
	PPRIML_ITEM curr = node->head;
	PPRIML_ITEM prev= NULL;
	void *curr_data = NULL;
	void *added_data = NULL;
	int res = -1;
	if (added && (added_data = added->get_data(added))) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(added_data, curr_data)) > 0 ) {
			prev= curr;
			curr = curr->next;
		}
		//match with the first item
		if (!prev && res == 0) {
			added->next = node->head->next;
			node->head->next = added;
			if (node->tail == node->head) {
				node->tail = added;
			}
			node->count++;
		}
		//smaller then first item
		else if (!prev && res < 0) {
			added->next = node->head;
			node->head = added;
			if (!node->tail) {
				node->tail = added;
			}
			node->count++;
		}
		//add to tail
		else if (res > 0) {
			prev->next = added;
			node->tail = added;
			node->count++;
		}
		else if (res == 0) {
			added->next = curr->next;
			curr->next = added;
			if(node->tail == curr) {
				node->tail = added;
			}
			node->count++;
		}
		else if (res < 0) {
			added->next = curr;
			prev->next = added;
			node->count++;
		}
		
	}
	else {
		return NULL;
	}
	
	node->currptr = node->head;
	return node;
	
}

static PPRIMLIST __primlist_remove_common (PPRIMLIST node, void *deleted, int (*cmp) (void *, void *)) {
	PPRIML_ITEM curr = node->head;
	PPRIML_ITEM prev= NULL;
	void *curr_data = NULL;
	PPRIML_ITEM tmp = NULL; 
	int res = -1;
	if (deleted) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) > 0 ) {
			prev= curr;
			curr = curr->next;
		}
		if (res) {
			return NULL;
		}
		do {
			if (!prev) {
				tmp = node->head;
				if (node->head == node->tail) {
					node->tail = node->head->next;
				}
				node->head = node->head->next;
				tmp->delete(tmp);
				curr = node->head;
				node->count--;
			}
			else {
				if (curr == node->tail)  {
					node->tail = node->tail->next;
				}
				tmp = curr;
				prev->next = curr->next;
				tmp->delete(tmp);
				curr = prev->next;
				node->count--;
			}
			
		}while(curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) == 0);
		//match with the first item
	}
	else {
		return NULL;
	}
	
	node->currptr = node->head;
	return node;
}

/*
https://www.geeksforgeeks.org/quicksort-on-singly-linked-list/
*/
PPRIML_ITEM __primlist_partition (PPRIML_ITEM head, PPRIML_ITEM end, PPRIML_ITEM *new_head, PPRIML_ITEM *right_tail, PPRIML_ITEM *new_end, int (*fn) (void *, void *)) {
	PPRIML_ITEM pivot = end;
	PPRIML_ITEM prev = NULL;
	PPRIML_ITEM curr = head;
	PPRIML_ITEM tail = pivot;
	
	while(curr && curr != pivot) {
		if (!pivot) {
			break;
		}
		else {
			if (fn(curr->get_data(curr), pivot->get_data(pivot)) < 0) {
				if ((*new_head) == NULL)  {
					//first smaller than pivot becomes new_head
					(*new_head) = curr;
				}
				prev = curr;
				curr = curr->next;
			}
			else {
				if (prev) {
					//move to right side of pivot
					prev->next = curr->next;
				}

				PPRIML_ITEM tmp = curr->next;
				curr->next = NULL;//unlink with the rest of the list
													//later invoker of this function
													//will relink it
				tail->next = curr;
				tail = curr;
				curr = tmp;
				
			}
		}
	}
	
	if (prev) {
		prev->next = NULL;
	}
	*right_tail = prev;
	
	if (!(*new_head)) {
		*new_head = pivot;
	}
	
	*new_end = tail;
	return pivot;
}

PPRIML_ITEM __prim_list_get_tail (PPRIML_ITEM curr) {
	while(curr && curr->next) {
		curr = curr->next;
	}
	return curr;
}

PPRIML_ITEM __primlist_quicksort_recur (PPRIML_ITEM head, PPRIML_ITEM end, int (*fn) (void *, void *)) {
	if (!head || head == end) {
		return head;
	}
	
	PPRIML_ITEM new_head = NULL;
	PPRIML_ITEM new_end = NULL;
	PPRIML_ITEM right_tail = NULL;
	
	PPRIML_ITEM pivot = __primlist_partition (head, end, &new_head, &right_tail, &new_end, fn);
	
	if (pivot && new_head != pivot) {
	//there are left side of pivot, sort them
		new_head = __primlist_quicksort_recur (new_head, right_tail, fn);
		PPRIML_ITEM tmp;
		if ((tmp = __prim_list_get_tail (new_head))) {
			tmp->next = pivot;
		}
		pivot->next = __primlist_quicksort_recur (pivot->next, new_end, fn);
	}
	//sort the right side of pivot

	return new_head;
}

void __primlist_quicksort (PPRIMLIST plist, int (*fn) (void *, void *)) {
	plist->head = __primlist_quicksort_recur (plist->head, plist->tail, fn);
	PPRIML_ITEM last = __prim_list_get_tail (plist->head);
	plist->tail = last;
}
static PPRIMLIST __primlist_ctor(PPRIMLIST plist) {
	if (plist) {
		__priml_item_ctor(&plist->priml_item);
		plist->currptr = NULL;
		plist->count = 0;
		plist->take = __primlist_take;
		plist->get_first_child = __primlist_getfirstchild;
		plist->get_next_child= __primlist_getnextchild;
		plist->set_data = __priml_item_set_data;
		plist->priml_item.primclass.delete =__primlist_delete;
		plist->detach = __primlist_detach;
		plist->delete = __primlist_delete;
		plist->get_data = __priml_item_get_data;
		plist->collect = __primlist_collect;
		plist->add_one = __primlist_add_one;
		plist->remove_one = __primlist_remove_one;
		plist->add_common = __primlist_add_common;
		plist->remove_common = __primlist_remove_common;
		plist->sort = __primlist_quicksort;
		plist->find_one = __primlist_find_one;
	}
	return plist;
}

PPRIMLIST newprimlist ()
{
	PPRIMLIST plist = (PPRIMLIST) calloc (1, sizeof (PRIMLIST));
	if (plist)
	{
		return __primlist_ctor(plist);
	}
	return plist;
}

static int __primtreeitem_add_last (struct primtree_item *root, struct primtree_item *addedchild)
{
	if (!root->head)
	{
		root->head = addedchild;
		root->tail = addedchild;
	}
	else
	{
		root->tail->next = addedchild;
		root->tail = addedchild;
	}
	addedchild->parent = root;
	return 0;
}

static int __primtreeitem_add_first(struct primtree_item *root, struct primtree_item *addedchild)
{
	if (!root->head)
	{
		root->head = addedchild;
		root->tail = addedchild;
	}
	else
	{
		addedchild ->next = root->head;
		root->head = addedchild;
		
	}
	addedchild->parent = root;
	return 0;
}
static PPRIMTREE_ITEM __primtreeitem_add_one (PPRIMTREE_ITEM node, PPRIMTREE_ITEM added, int (*cmp) (void *, void *)) {
	if (node) {
		PPRIMTREE_ITEM curr = node->head;
		PPRIMTREE_ITEM prev= NULL;
		void *curr_data = NULL;
		void *added_data = NULL;
		PPRIMTREE_ITEM tmp = NULL;
		int res = -1;
		if (added && (added_data = added->get_data(added))) {
			while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(added_data, curr_data)) > 0 ) {
				prev= curr;
				curr = curr->next;
			}
			//match with the first item
			if (!prev && res == 0) {
				added->next = node->head->next;
				tmp = node->head;
				node->head = added;
				if (node->tail == tmp) {
					node->tail = added;
				}
				tmp->delete(tmp);
			}
			//smaller then first item
			else if (!prev && res < 0) {
				added->next = node->head;
				node->head = added;
				if (node && !node->tail) {
					node->tail = added;
				}
			}
			//add to tail
			else if (res > 0) {
				prev->next = added;
				node->tail = added;
			}
			else if (res == 0) {
				tmp = curr;
				added->next = curr->next;
				if(node->tail == curr) {
					node->tail = added;
				}
				prev->next = added;
				tmp->delete(tmp);
			}
			else if (res < 0) {
				added->next = curr;
				prev->next = added;
			}
		}
		else {
			return NULL;
		}
		added->parent = node;
		node->curr = node->head;
		return node;
	}
	return NULL;
}

static PPRIMTREE_ITEM __primtreeitem_remove_one (PPRIMTREE_ITEM node, void *deleted, int (*cmp) (void *, void *)) {
	PPRIMTREE_ITEM curr = node->head;
	PPRIMTREE_ITEM prev= NULL;
	void *curr_data = NULL;
	PPRIMTREE_ITEM tmp = NULL; 
	int res = -1;
	if (deleted) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) > 0 ) {
			prev= curr;
			curr = curr->next;
		}
		if (res) {
			return NULL;
		}
			//match with the first item
		else if (!prev) {
			tmp = node->head;
			if (node->head == node->tail) {
				node->tail = node->head->next;
			}
			node->head = node->head->next;
			tmp->delete(tmp);
		}
		else {
			if (curr == node->tail)  {
				node->tail = node->tail->next;
			}
			tmp = curr;
			prev->next = curr->next;
			tmp->delete(tmp);
		}
	}
	else {
		return NULL;
	}
	
	node->curr = node->head;
	return node;
}


static PPRIMTREE_ITEM __primtreeitem_find_one (PPRIMTREE_ITEM node, void *to_find, int (*cmp) (void *, void *)){
	PPRIMTREE_ITEM curr = node->head;
	void *curr_data = NULL;
	int res = -1;
	if (to_find) {
		while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(to_find, curr_data)) > 0 ) {
			curr = curr->next;
		}
		if (res) {
			return NULL;
		}
		else {
			return curr;
		}
	}
	return NULL;
}

static PPRIMTREE_ITEM __primtreeitem_add_common(PPRIMTREE_ITEM node, PPRIMTREE_ITEM added, int (*cmp) (void *, void *)) {
	PPRIMTREE_ITEM curr = node->head;
	PPRIMTREE_ITEM prev = NULL;
	void *curr_data;
	void *added_data;
	int res = -1;
	
	if (added && (added_data = added->get_data(added))) {
		while(curr && (curr_data = curr->get_data(curr)) && (res = cmp(added_data, curr_data)) > 0){
			prev = curr;
			curr = curr->next;
		}
		
		if(!prev && res == 0) {
			added->next = node->head->next;
			node->head->next = added;
			if (node->tail == node->head) {
				node->tail = added;
			}
		}
		else if (!prev && res < 0) {
			added->next = node->head;
			node->head = added;
			if (!node->tail) {
				node->tail = added;
			}
		}
		else if (res > 0) {
			prev->next = added;
			node->tail = added;
		}
		else if (res == 0) {
			added->next = curr->next;
			curr->next = added;
			if (node->tail == curr) {
				node->tail = added;
			}
		}
		else if (res < 0) {
			added->next = curr;
			prev->next = added;
		}
		node->curr = node->head;
		return node;
	}
	else {
		return NULL; 
	}
}

static PPRIMTREE_ITEM __primtreeitem_remove_common(PPRIMTREE_ITEM node, void *deleted, int (*cmp) (void *, void *)) {
	PPRIMTREE_ITEM curr = node->head;
	PPRIMTREE_ITEM prev = NULL;
	void *curr_data = NULL;
	PPRIMTREE_ITEM tmp = NULL;
	int res = -1;
	if (deleted) {
		while(curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) > 0) {
			prev = curr;
			curr = curr->next;
		}
		if(res) {
			return NULL;
		}
		do {
			if (!prev) {
				tmp = node->head;
				if (node->head == node->tail) {
					node->tail = node->head->next;
				}
				node->head = node->head->next;
				tmp->delete(tmp);
				curr = node->head;
			}
			else {
				if (curr == node->tail) {
					node->tail = node->tail->next;
				}
				tmp = curr;
				prev->next = curr->next;
				tmp->delete(tmp);
				curr = prev->next;
			}
		}while (curr && (curr_data = curr->get_data(curr)) && (res = cmp(deleted, curr_data)) == 0);
		node->curr = node->head;
		return node;
	}
	else {
		return NULL;
	}
}
static struct primtree_item * __primtreeitem_getparent (struct primtree_item *node)
{
	if (node){
		return node->parent;
	}
	else {
		return NULL;
	}
}

static struct primtree_item * __primtreeitem_getfirstchild (struct primtree_item *node) {
	struct primtree_item *ret=node->head;
	if (node->head){
		node->curr = node->head->next;
	}
	return ret;	
}

static struct primtree_item * __primtreeitem_getnextchild (struct primtree_item *node)
{
	struct primtree_item *ret = node->curr;
	if (node->curr){
		node->curr = node->curr->next;
	}
	return ret;
}

/**
 * detach head and return detached head
 */
static struct primtree_item * __primtreeitem_detach_head (struct primtree_item *node)
{
	struct primtree_item * ret = NULL;
	if(node && node->head)
	{
		ret = node->head;
		node->head = (PPRIMTREE_ITEM) node->head->next; 
	}
	return ret;
}

/**
 * delete list items 
 */
static int __primtreeitem_listdelete(struct primtree_item *node)
{
	PPRIML_ITEM curr = NULL;
	void *data = NULL;
	while (( curr = node->list.take(&node->list)))
	{
		curr-> delete(curr);
		node->list.count --;
	}
	if ((data = node->list.get_data(&node->list))){
		free(data);
	}
	return 0;
}

static PPRIMTREE_ITEM  __primtreeitem_detach_node(struct primtree_item *node, struct primtree_item *detached)
{
	PPRIMTREE_ITEM curritem = node->head;
	PPRIMTREE_ITEM prev = NULL;
	while (curritem)
	{
		if (detached == curritem) {
			if (prev){
				prev->next = curritem->next;
			}
			else {
				node->head = curritem->next;
			}
			return NULL;
		}
		prev = curritem;
		curritem = curritem->next;
	}
	return detached;
}

static int __primtreeitem_delete(void *arg1) {
	PPRIMTREE_ITEM pitem = (PPRIMTREE_ITEM) arg1;
	PPRIMTREE_ITEM child = NULL;
	if (pitem){
		while((child = __primtreeitem_detach_head(pitem))) {
			__primtreeitem_delete(child);
		}
	}
	__primtreeitem_listdelete(pitem);
	free(pitem);
	return 0;
}

static int __primtreeitem_collect(PPRIMTREE_ITEM in, int  (*filter_fn) (void *), void (*collect_fn)(void *, void *), void *collected) {
	int counter = 0;
	void *data = NULL;
	if ((data = in->get_data(in)) && filter_fn(data)) {
		collect_fn(data, collected);
	}
	PPRIMTREE_ITEM curr = __primtreeitem_getfirstchild(in);
	while(curr) {
		__primtreeitem_collect(curr, filter_fn, collect_fn, collected);
		curr  = __primtreeitem_getnextchild(in);
	}
	return counter;
}

PPRIMTREE_ITEM __primtreeitem_partition (PPRIMTREE_ITEM head, PPRIMTREE_ITEM end, PPRIMTREE_ITEM *new_head, PPRIMTREE_ITEM *right_tail, PPRIMTREE_ITEM *new_end, int (*fn) (void *, void *)) {
	PPRIMTREE_ITEM pivot = end;
	PPRIMTREE_ITEM prev = NULL;
	PPRIMTREE_ITEM curr = head;
	PPRIMTREE_ITEM tail = pivot;
	
	while(curr && curr != pivot) {
		if (pivot) {
			if (fn(curr->get_data(curr), pivot->get_data(pivot)) < 0) {
				if ((*new_head) == NULL)  {
					//first smaller than pivot becomes new_head
					(*new_head) = curr;
				}
				prev = curr;
				curr = curr->next;
			}
			else {
				if (prev) {
					//move to right side of pivot
					prev->next = curr->next;
				}

				PPRIMTREE_ITEM tmp = curr->next;
				curr->next = NULL;//unlink with the rest of the list
													//later invoker of this function
													//will relink it
				tail->next = curr;
				tail = curr;
				curr = tmp;
				
			}
		}
	}
	
	if (prev) {
		prev->next = NULL;
	}
	*right_tail = prev;
	
	if (!(*new_head)) {
		*new_head = pivot;
	}
	
	*new_end = tail;
	return pivot;
}

PPRIMTREE_ITEM __primtreeitem_get_tail (PPRIMTREE_ITEM curr) {
	while(curr && curr->next) {
		curr = curr->next;
	}
	return curr;
}

PPRIMTREE_ITEM __primtreeitem_quicksort_recur (PPRIMTREE_ITEM head, PPRIMTREE_ITEM end, int (*fn) (void *, void *)) {
	if (!head || head == end) {
		return head;
	}
	
	PPRIMTREE_ITEM new_head = NULL;
	PPRIMTREE_ITEM new_end = NULL;
	PPRIMTREE_ITEM right_tail = NULL;
	
	PPRIMTREE_ITEM pivot = __primtreeitem_partition (head, end, &new_head, &right_tail, &new_end, fn);
	
	if (pivot && new_head != pivot) {
	//there are left side of pivot, sort them
		new_head = __primtreeitem_quicksort_recur (new_head, right_tail, fn);
		PPRIMTREE_ITEM tmp;
		if ((tmp = __primtreeitem_get_tail (new_head))) {
			tmp->next = pivot;
		}
		//sort the right side of pivot
		pivot->next = __primtreeitem_quicksort_recur (pivot->next, new_end, fn);
	}

	return new_head;
}

void __primtreeitem_quicksort (PPRIMTREE_ITEM item, int (*fn) (void *, void *)) {
	item->head = __primtreeitem_quicksort_recur (item->head, item->tail, fn);
	PPRIMTREE_ITEM last = __primtreeitem_get_tail (item->head);
	item->tail = last;
}

PPRIMTREE_ITEM primtreeitem_ctor(PPRIMTREE_ITEM pitem) {
	if (pitem) {
		__primlist_ctor(&pitem->list);
		pitem->get_parent = __primtreeitem_getparent;
		pitem->get_first_child= __primtreeitem_getfirstchild;
		pitem->get_next_child= __primtreeitem_getnextchild;
		pitem->detach_head= __primtreeitem_detach_head;
		pitem->detach_node= __primtreeitem_detach_node;
		pitem->add_last= __primtreeitem_add_last;
		pitem->add_first= __primtreeitem_add_first;
		pitem->list.priml_item.primclass.delete = __primtreeitem_delete;
		pitem->set_data = __priml_item_set_data;
		pitem->delete= __primtreeitem_delete;
		pitem->get_data = __priml_item_get_data;
		pitem->collect = __primtreeitem_collect;
		pitem->add_one = __primtreeitem_add_one;
		pitem->remove_one = __primtreeitem_remove_one;
		pitem->find_one = __primtreeitem_find_one;
		pitem->add_common = __primtreeitem_add_common;
		pitem->remove_common = __primtreeitem_remove_common;
		pitem->sort = __primtreeitem_quicksort;
	}
	return pitem;
}

PPRIMTREE_ITEM newprimtreeitem() {
	PPRIMTREE_ITEM pitem = (PPRIMTREE_ITEM) calloc (1, sizeof(PRIMTREE_ITEM));
	if (pitem) {
		primtreeitem_ctor(pitem);
	}
	return pitem;
}

PDLIST_ITEM dlist_item_set_data(PDLIST_ITEM entry, void *data) {
	if(entry) {
		entry->data = data;
	}
	return entry;
}

void  dlist_item_set_data_remove_fn(PDLIST_ITEM entry, int (*data_remove_fn) (void *)) {
	if (entry) {
		entry->data_remove_fn = data_remove_fn;
	}
}

int  dlist_item_delete(PDLIST_ITEM entry) {
	if (entry) {
		if (entry->data_remove_fn) {
			entry->data_remove_fn(entry->data);
		}
		else {
			free(entry->data);
		}
		entry->prev = entry->next = NULL;
		free(entry);
		return 1;
	}	
	return 0;
}



PDLIST_ITEM  dlist_item_ctor(PDLIST_ITEM in) {
	if (in) {
		in->set_data = dlist_item_set_data;
		in->set_data_remove_fn = dlist_item_set_data_remove_fn;
		in->delete = dlist_item_delete;
	}
	return in;
}

PDLIST_ITEM new_dlist_item() {
	PDLIST_ITEM ret = (PDLIST_ITEM )calloc(1, sizeof(DLIST_ITEM ));
	if (ret) {
		return dlist_item_ctor(ret);
	}
	return ret;
}

PDLIST_ITEM dlist_push(PDLIST lst, PDLIST_ITEM item) {
	if(lst && lst->tail) {
		lst->tail->next = item;
		item->prev = lst->tail;
		lst->tail = item;
	}
	else {
		lst->head = lst->tail = item;
	}
	return item;
}

PDLIST_ITEM dlist_pop(PDLIST lst) {
	if (lst && lst->tail) {
		PDLIST_ITEM to_ret = lst->tail;
		if (to_ret->prev) {
			to_ret->prev->next = NULL;
			lst->tail = to_ret->prev;
		}
		else  {//also a head
			lst->head = NULL;
			lst->tail = NULL;
		}
		to_ret->prev = NULL;
		return to_ret;
	}
	return NULL;
}

int dlist_delete(PDLIST lst) {
		PDLIST_ITEM curr = NULL;
		PDLIST_ITEM next = NULL;

	if (lst) {
		curr = lst->head;
		while(curr) {
				curr->delete(curr);
				next = curr->next;
				curr = next;
		}
		lst->head = lst->tail = NULL;
		free(lst);
		return 1;
	}
	return 0;
}

PDLIST  dlist_ctor(PDLIST lst) {
	if (lst) {
		lst->head = lst->tail = NULL;
		lst->push = dlist_push;
		lst->pop = dlist_pop;
		lst->delete = dlist_delete;
	}
	return lst;
}

PDLIST new_dlist() {
	PDLIST ret = (PDLIST) calloc(1, sizeof(DLIST));
	if (ret) {
		dlist_ctor(ret);
	}
	return ret;
}
char *copy_string(char **target, char *in) {
	char *copied ;
	if (!*target) {
		copied = calloc(1, strlen(in) + 2);
		if (copied) {
			strncpy(copied, in, strlen(in));
			*target = copied;
			return copied;
		}
		else {
			return NULL;
		}
	}
	else {
		*target = realloc(*target, strlen(in) + 2);
		if (*target) {
			strncpy(*target, in, strlen(in));
			return *target;
		}
		else {
			return NULL;
		}
		
	}
}

char *dlist_serialize(PDLIST dlist, char *out, char *delimiter) {
	if (dlist) {
		PDLIST_ITEM curr = dlist->head;
		while(curr) {
			if (curr->data){
				strcat(out, (char *) curr->data);
				if(curr->next) {
					strcat(out, delimiter);
				}
			}
			curr = curr->next;
		}
		return out;
	}
	else {
		return NULL;
	}
}

void reverse_string_inplace(char inbuf[], int len) {
	int tmp;
	int i;
	if (len<=1)
		return;
	
	for (i = 0; i < len/2; i++){
		tmp = inbuf[i];
		inbuf[i] = inbuf[len - i - 1];
		inbuf[len - i - 1] = tmp;
	}
}

void get_last_path(char *path, int path_len, char *out) {
	int j = 0;
	int elem_len = 0;
	for(int i = path_len - 1; i >= 0; i --){
		if(path[i] == '/')
			break;
		else {
			out[j++] = path[i];
		}
	}
	elem_len = strlen(out);
	reverse_string_inplace(out, elem_len);
}

#ifdef _PARSER_CLASS_TEST_
#include <stdio.h>
#include <stdlib.h> 
#include <getopt.h>
#include <string.h>
#include "yxml.h"
void split_elem_attrib(char *path, char *out_elem, char* out_attrib) {
	int last_delim = 0;
	int i = 0;
	int j = 0;
	
	while(path[i]){
		if (path[i] == '/'){
			last_delim = i;
		}
		out_elem[j] = path[i];
		i++;
		j++;
	}
	if (last_delim == 0){
		i = last_delim;
	}
	else {
		out_elem[last_delim ] = 0;
		i = last_delim + 1;
	}
	j = 0;
	while(out_elem[i]){
		out_attrib[j] = out_elem[i];
		out_elem[i] = 0;
		i++;
		j++;
	}

}

typedef struct {
	char *name;
	char *value;
} map_struct;

map_struct *new_map_struct() {
	return calloc(1, sizeof(map_struct));
}

map_struct *map_set_name(map_struct *in, char *input) {
	if (copy_string(&in->name, input)){
		return in;
	}
	return NULL;
}

map_struct *map_set_value(map_struct *in, char *input) {
	if (copy_string(&in->value, input)){
		return in;
	}
	return NULL;
}

int map_free(map_struct *in) {
	if (!in)
		return 1;
	if (in->name) {
		free(in->name);
		in->name = NULL;
	}
	if (in->value) {
		free(in->value);
		in->value = NULL;
	}
	free(in);
	return 1;
}

void map_print(map_struct *in, int ident) {
	for(int i = 0; i < ident; i++){
		fprintf(stdout, "\t");
	}
	if(!in) {
		fprintf(stdout, "NULL \n");
		return;
	}
	else if(!in->name){
		fprintf(stdout, "name: NULL");
	}
	else {
		fprintf(stdout, "name: %s", in->name);
	}
	if(!in->value) {
		fprintf(stdout, "\tvalue: NULL");
	}
	else {
		fprintf(stdout, "\tvalue: %s", in->value);
	}
	fprintf(stdout, "\n");
}

void priml_item_print(PPRIML_ITEM item, int ident) {
	if (item && item->data) {
		map_print((map_struct *) item->data, ident);
	}
}

void primlist_print(PPRIMLIST list, int ident) {
	PPRIML_ITEM currchild = NULL;
	char *data= list->get_data(list);
	if(data) {
		for(int i = 0 ; i < ident; i++) {
			fprintf(stdout, "\t");
		}
		fprintf(stdout, "%s\n", data);
	}
	currchild =  list->get_first_child(list);
	while(currchild) {
		priml_item_print(currchild, ident + 1);
		currchild = list->get_next_child(list);
	}
}

void primtreeitem_print(PPRIMTREE_ITEM item, int ident) {
	if (item) {
		primlist_print(&item->list, ident);
		PPRIMTREE_ITEM currchild = NULL;
		currchild = item->get_first_child(item);
		while(currchild) {
			primtreeitem_print(currchild, ident + 1);
			currchild = item->get_next_child(item);
		}
	}
}

int fn_map_value_cmp_by_name(map_struct *in, map_struct *out) {
	if (!out) {
		return 0;
	}
	return strcmp(in->name, out->name);
}	

int fn_map_value_cmp_by_value(map_struct *in1, map_struct *in2) {
	if (in1 && in2) {
		return strcmp(in1->value, in2->value);
	}
	return 0;
}

int xmls_unmarshall(char *xml_string, PRIMTREE_ITEM *root_tree){
	char *doc_p = xml_string;
	PRIMTREE_ITEM *tree_active= root_tree;
	PRIMTREE_ITEM *tree_current;
	PPRIML_ITEM new_item;
	
	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	char tmp[1024] = {0};
	map_struct  *map_active = NULL;
	char *elem_name = NULL; 
	
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				elem_name = NULL;
				tree_current = newprimtreeitem();
				copy_string(&elem_name, xml_elem.elem);
				tree_current->set_data(tree_current, elem_name);
				tree_active->add_one(tree_active, tree_current, (int (*) (void *, void *))strcmp);
				tree_active = tree_current;
				map_active = NULL;
				break;

			case YXML_ELEMEND:
				elem_name = NULL;
				//primtreeitem_print(tree_active, 0);
				tree_active = tree_active->parent;
				map_active = NULL;
				break;
				
			case YXML_ATTRSTART:
				map_active = new_map_struct();
				map_active = map_set_name(map_active, xml_elem.attr);
				break;
				
			case YXML_ATTRVAL:
				strcat(tmp, xml_elem.data);
				break;
				
			case YXML_ATTREND:
				map_set_value(map_active, tmp);
				memset(tmp, 0, sizeof(tmp));
				new_item = newpriml_item();
				new_item->set_data_remove_fn(new_item, (int (*) (void *))map_free);
				new_item->set_data(new_item, map_active);
				tree_active->list.add_one(&tree_active->list, new_item, (int (*) (void *, void *))fn_map_value_cmp_by_name);
				map_active = NULL;
				break;
				
			default: 
				
				break;
		}
		doc_p++;
	}
	return 0;
}

int  xmlt_marshall(PPRIMTREE_ITEM node, char *output, int outmax, int curr_len) {
	int i = curr_len;
	int j = 0;
	char *data;
	PPRIMTREE_ITEM  curr = NULL;
	map_struct *ms;
	if (node) {
		if ((data = node->get_data(node))) {
			if (i + 1 < outmax) {
				output[i++] = '<';
				while ((i + 1 < outmax) && data[j]){
					output[i++] = data[j++];
				}
				if (i + 1 < outmax) {
					output[i++] = ' ';
					PPRIML_ITEM item = node->list.get_first_child(&node->list);
					while (item) {
						if ((ms = (map_struct *)item->get_data(item))) {
							j = 0;
							while (i + 1 < outmax && ms->name[j]){
								output[i++] = ms->name[j++];
							}
							if (i + 4 < outmax ) {
								output[i++] = '=';
							}
							else {
								return i;
							}
							
							if (i + 3 < outmax ) {
								output[i++] = '"';
							}
							else {
								return i;
							}
							
							j = 0;
							while (i + 2 < outmax && ms->value[j]){
								output[i++] = ms->value[j++];
							}
							
							if (i + 2 < outmax) {
								output[i++] = '"';
							}
							if (i + 1 < outmax ) {
								output[i++] = ' ';
							}
							else {
								return i;
							}
						}
						item = node->list.get_next_child(&node->list);
					}
					if (i + 1 < outmax ) {
						output[i++] = '>';
					}
					else {
						return i;
					}
				}
			}
			else {
				return i;
			}
		}
		curr = node->get_first_child(node);
		while(curr) {
			i = xmlt_marshall(curr, output, outmax, i);
			curr = node->get_next_child(node);
		}
		if ((data = node->get_data(node))) {
			if (i + 3 < outmax ) {
				j =  0;
				output[i++] = '<';
				output[i++] = '/';
				while((i + 1 < outmax) && data[j]) {
					output[i++] = data[j++];
				}
				output[i++] = '>';
			}
			else {
				return i;
			}
		}
	}
	return i;
}

PPRIMTREE_ITEM xmlt_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

		while(*p_path == ' '){
			p_path ++;
		}

		start_path = p_path;
		while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
			p_path ++;
		}

		/* empty path or end of string */
		if (start_path == p_path){
			return node;
		}

		end_path = p_path;

		while (*p_path == ' ')
			p_path ++;

		if (*p_path == '/')
			p_path ++;

		strncpy(elem_name, start_path, end_path - start_path);
		
		if (! (next_tree = node->find_one(node, elem_name, (int (*) (void *, void *))strcmp))) {
			return next_tree;
		}
		else {
			return xmlt_find_element(next_tree, p_path);
		}
}
/*first implement tree add sorted*/
PPRIMTREE_ITEM xmlt_add_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	char *new_string = NULL;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

	while(*p_path == ' '){
		p_path ++;
	}

	start_path = p_path;
	while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
		p_path ++;
	}

	/* empty path or end of string */
	if (start_path == p_path){
		return node;
	}

	end_path = p_path;

	while (*p_path == ' ')
		p_path ++;

	if (*p_path == '/')
		p_path ++;

	strncpy(elem_name, start_path, end_path - start_path);
	if (! (next_tree = node->find_one(node, elem_name, (int (*) (void *, void *))strcmp))) {
		new_string = copy_string(&new_string, elem_name);
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, new_string);
		node->add_one(node, next_tree, (int (*) (void *, void *))strcmp);
	}
	return xmlt_add_element(next_tree, p_path);
	
}

int negate_strcmp(char *a, char *b) {
	int ret  = strcmp(a, b);
	return -1 *ret;
}

void usage(char *app) {
	fprintf (stdout, "usage : %s i:pf:c:hsr:mudta:e:uj:o:g:l:x:w:\n\
		-i --       : input to parse xml\n\
		-p -- print : current active tree\n\
		-f          : find element , to find and element, parameter is path_to_the element\n\
		-s --serialize : print current tree to xml format\n\
		-t --top    : set root as active element\n\
		-r --remove : remove an attribute from current active tree\n\
		-o          : obliterate/remove some attributes with specified attribute names\n\
		-d          : delete active element. Active element is change to root.\n\
		-x          : expunge/delete some elements matching with value.\n\
		-a          : add an attribute to active element, the format is attributename=attributevalue, \n\
			          : if attribute name exists will replace attribute value with new one\n\
		-j          : add an attribute to active element, the format is attributename=attributevalue. \n\
			          : will add to list despites attribute name exists before\n\
		-h --help   : this help\n\
		-c --create : insert and element to current active tree. Will replace old value if key exists \n\
		-l          : lodge/insert element to current active tree. Will not replace old value if key exists \n\
		-m --memtest : memory leak test, loop create update and delete test\n\
		-e           : echo something\n\
		-g           : group/sort attribute [name|value]\n\
		-w           : group/sort child of current tree [a|d] ascending or descending \n\
", app);
}
int main (int argc, char **argv) {
	int c;
	char buff[2048] = {0};
	char elem[2048] = {0};
	char attrib[2048] = {0};
	char *name;
	char *new_string = NULL;
	char *value;
	PPRIMTREE_ITEM root_tree = newprimtreeitem();
	PPRIMTREE_ITEM active_tree = root_tree;
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"i", required_argument, 0, 'i'},
			{"print", no_argument, 0, 'p'},
			{"collect", required_argument, 0, 'g'},
			{"help", no_argument, 0, 'h'},
			{"serialize", no_argument, 0, 's'},
			{"remove", required_argument, 0, 'r'},
			{"memtest", no_argument, 0, 'm'},
			{0, 0, 0, 0}
		};

		c  = getopt_long(argc, argv, "i:pf:c:hsr:mudta:e:j:o:g:l:x:w:", long_options, &option_index);

		if (c == -1){
			break;
		}

		switch (c) {
			case 'i' :
				strcpy(buff, optarg);
				xmls_unmarshall(buff, active_tree);
				break;
			case 'p' :
				if(active_tree){
					primtreeitem_print(active_tree, 0);
				}
				else{
					fprintf(stdout, "empty tree");
				}
				break;
			case 'f' :
				if (active_tree) {
					strcpy(buff, optarg);
					active_tree = xmlt_find_element(active_tree, buff);
				}
				else {
					fprintf(stdout, "not found\n");
				}
				break;
			case 'c' :
				strcpy(buff, optarg);
				active_tree = xmlt_add_element(active_tree, buff);
				break;
			case 'l':
				new_string = NULL;
				strcpy(buff, optarg);
				PPRIMTREE_ITEM prev_active = active_tree;
				if (strstr(buff, "/")) {
					split_elem_attrib(buff, elem, attrib);
					active_tree = xmlt_find_element(active_tree, elem);
				}
				else {
					strcpy(attrib, optarg);
				}
				if (active_tree) {
					PPRIMTREE_ITEM pitem = newprimtreeitem();
					new_string = copy_string(&new_string, attrib);
					pitem->set_data(pitem, (void *)new_string);
					active_tree = active_tree->add_common(active_tree, pitem, (int (*) (void *, void *))strcmp);
				}
				else {
					fprintf(stdout, "element not found\n");
					active_tree = prev_active;
				}
				break;
			case 'x':
				if (strstr(buff, "/")) {
					split_elem_attrib(buff, elem, attrib);
					active_tree = xmlt_find_element(active_tree, elem);
				}
				else {
					strcpy(attrib, optarg);
				}
				if (active_tree) {
					active_tree->remove_common(active_tree, attrib, (int (*) (void *, void *))strcmp);
					active_tree = prev_active;
				}
				else {
					fprintf(stdout, "element not found\n");
					active_tree = prev_active;
				}
				
				break;
			case 'e' :
				strcpy(buff, optarg);
				fprintf(stdout, "%s\n", buff);
				break;
			case 'h':
				usage(argv[0]);
				root_tree->delete(root_tree);
				return 0;
			case 's':
				memset(buff, 0, 2048);
				xmlt_marshall(active_tree, buff, 2048, 0);
				fprintf(stdout, "%s\n", buff);
				break;
			case 'r' :
				name = NULL;
				value = NULL;
				if (active_tree) {
					strcpy(buff, optarg);
					int i = 0;
					map_struct *active_map = new_map_struct();
					while(buff[i] &&  buff[i] != '=') {
						i++;
					}
					name = buff;
					map_set_name(active_map, name);
					active_tree->list.remove_one(&active_tree->list, active_map, (int (*) (void *, void *))fn_map_value_cmp_by_name);
					map_free(active_map);
					active_map = NULL;
					
				}
				break;
			case 'o' :
				name = NULL;
				value = NULL;
				if (active_tree) {
					strcpy(buff, optarg);
					int i = 0;
					map_struct *active_map = new_map_struct();
					while(buff[i] &&  buff[i] != '=') {
						i++;
					}
					name = buff;
					map_set_name(active_map, name);
					active_tree->list.remove_common(&active_tree->list, active_map, (int (*) (void *, void *))fn_map_value_cmp_by_name);
					map_free(active_map);
					active_map = NULL;
					
				}
				break;
			case 't' :
				active_tree = root_tree;
				break;
			case 'd' :
				if (active_tree) {
					PPRIMTREE_ITEM parent = active_tree->get_parent(active_tree);
					if (parent) {
						parent->detach_node(parent, active_tree);
					}
					active_tree->delete(active_tree);
					active_tree = root_tree;
				}
				break;
			case 'a' :
				name = NULL;
				value = NULL;
				if (active_tree) {
					strcpy(buff, optarg);
					int i = 0;
					PPRIML_ITEM new_item = newpriml_item();
					map_struct *active_map = new_map_struct();
					while(buff[i] &&  buff[i] != '=') {
						i++;
					}
					
					if (buff[i] == '=') {
						buff[i] = 0;
						value = &buff[i + 1];
						map_set_value(active_map, value);
					}
					name = buff;
					map_set_name(active_map, name);
					new_item->set_data_remove_fn(new_item, (int (*) (void *))map_free);
					new_item->set_data(new_item, active_map);
					active_tree->list.add_one(&active_tree->list, new_item, (int (*) (void *, void *))fn_map_value_cmp_by_name);
					
				}
				break;
			case 'j' :
				name = NULL;
				value = NULL;
				if (active_tree) {
					strcpy(buff, optarg);
					int i = 0;
					PPRIML_ITEM new_item = newpriml_item();
					map_struct *active_map = new_map_struct();
					while(buff[i] &&  buff[i] != '=') {
						i++;
					}
					
					if (buff[i] == '=') {
						buff[i] = 0;
						value = &buff[i + 1];
						map_set_value(active_map, value);
					}
					name = buff;
					map_set_name(active_map, name);
					new_item->set_data_remove_fn(new_item, (int (*) (void *))map_free);
					new_item->set_data(new_item, active_map);
					active_tree->list.add_common(&active_tree->list, new_item, (int (*) (void *, void *))fn_map_value_cmp_by_name);
				}
				break;
			case 'm' :
				optind = 1;
				usleep(100000);
				break;
			case 'g' :
				if (active_tree) {
					if (!strcmp(optarg, "value")) {
						active_tree->list.sort(&active_tree->list, (int (*) (void *, void *))fn_map_value_cmp_by_value);
					}
					else if (!strcmp(optarg, "name")) {
						active_tree->list.sort(&active_tree->list, (int (*) (void *, void *))fn_map_value_cmp_by_name);
					}
				}
				break;
			case 'w' :
				if (active_tree) {
					if (!strcmp(optarg, "a")) {
						active_tree->sort(active_tree, (int (*) (void *, void *))strcmp);
					}
					else if (!strcmp(optarg, "d")) {
						active_tree->sort(active_tree, (int (*) (void *, void *))negate_strcmp);
					}
				}
				break;
		}
		
	}

	root_tree->delete(root_tree);
	
}



#endif
