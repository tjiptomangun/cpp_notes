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
*/

#ifndef __PARSERCLASS_H__
#define __PARSERCLASS_H__

#define MAX_NAME_LENGTH	40 
/*our language specific data structure*/
/*section, property, value*/ 
#define CLASS_CLASS 0
#define CLASS_L_ITEM 1
#define CLASS_LIST 2
#define CLASS_PROPERTY 3
#define CLASS_CIRCULARITEM 4
#define CLASS_CIRCULARLIST 5
#include <stdio.h>

/**
 * NAME		: print_identation
 * DESCRIPTION	: print identation ident times
 */
extern void print_identation (int ident);
/**
 * NAME		: trimstring
 * DESCRIPTION	: remove blank spaces (' ') in the beginning
 * 		  and end of string
 */
extern char *trimstring (char *instr);
typedef struct class 
{
	struct  class *this;
	char    name[MAX_NAME_LENGTH];
	int	type; 
	int (*printattributes) (struct class *, int);
	int (*delete) (struct class *);
	int (*writetofile ) (struct class *, FILE *);
}STRUCT_CLASS, CLASS, *PSTRUCT_CLASS, *PCLASS; 

extern PCLASS newclass (char *name);
extern int __class_delete (PCLASS p);
extern void __class_ctor(PCLASS p, char *name); 

typedef struct l_item
{
	struct class class;
	struct l_item *next;
}L_ITEM, *PL_ITEM; 

extern PL_ITEM newl_item (char *name);

typedef struct list
{
	L_ITEM     l_item;
	PL_ITEM    head;
	PL_ITEM    tail;
	PL_ITEM   currptr;
	int          count;
	void (*add) (struct list *, PL_ITEM);
	PL_ITEM (*take) (struct list *);
	PL_ITEM (*takename) (struct list *, char *); 
	PL_ITEM (*getname) (struct list *, char *); 
	PL_ITEM (*getfirstchild) (struct list *); 
	PL_ITEM (*getnextchild) (struct list *); 
	PL_ITEM (*detach) (struct list *, PL_ITEM);
	int (*addproperty) (struct list *, char *, char *);
}LIST, *PLIST;

extern PLIST newlist (char *list_name);
extern void __list_ctor (PLIST , char *);
extern void list_resetlist (PLIST plist, char *list_name);

typedef struct property
{
	L_ITEM l_item ;
	char value[256]; 
	int (*setvalue) (struct property *, char*);
}PROPERTY, *PPROPERTY; 

extern PPROPERTY newproperty (char *name);
extern PPROPERTY newproperty2 (char *name, char *value);


typedef struct stack_ptr
{
	int top;
	void * c[100];
	int (* init)(struct stack_ptr *);
	int (* push) (struct stack_ptr *, void *);
	void * (* pop) (struct stack_ptr *);
	int (* is_empty) (struct stack_ptr *);
}STACK_PTR, *PSTACK_PTR; 

extern PSTACK_PTR newstackptr ();


typedef struct miniparser
{
	FILE *input;
	char *tokenlist;
	int (*parse) (struct miniparser *, void *); 
}MINIPARSER, *PMINIPARSER;

typedef struct fileparser
{
	FILE *input; 
	int (*parse) (struct fileparser*, PLIST *); 
}FILEPARSER, *PFILEPARSER;

extern PMINIPARSER newminiparser (FILE *in, char *tokenlist, int (*f)(PMINIPARSER, void*));
extern PFILEPARSER newfileparser (FILE *in, int (*f)(PFILEPARSER, PLIST *));

typedef struct charparser
{
	char *input;
	char *tokenlist;
	int  currpos;
	int  charlen;
	int (*parse) (struct charparser *, PLIST *); 
}CHARPARSER, *PCHARPARSER;
extern PCHARPARSER newcharparser (char *in, int inlen, char *tokenlist, int (*f)(PCHARPARSER, PLIST *));

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
extern int stream_gettoken (FILE *fp, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read);



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
extern int stream_expectnexttoken (FILE *fp, char *tokenlist, char *buff, 
	int max_buffsize, int *readlen,  char *expected_token);


extern int string_gettoken (char *input, char tokenlist[], char * buff, 
	int max_buffsize, int *length_read);


extern int string_expectnexttoken (char *input, char *tokenlist, char *buff, 
	int max_buffsize, int *readlen,  char *expected_token);

/*
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
 * RETURNS
 *			: on return, return length of interpretted string
 *
 */

extern int parse_env_str (char *instring, char *newenv, int newenv_max );

typedef struct circularitem
{
	CLASS class;
	struct circularitem *next;
	struct circularitem *prev;
	int   datasize;
	void *data;
}CIRCULARITEM, *PCIRCULARITEM;

typedef struct circularlist
{
	CLASS class;
	PCIRCULARITEM current;
	int count;
	void (*add)  (struct circularlist *, PCIRCULARITEM);
	int (*remove) (struct circularlist *, PCIRCULARITEM);
	PCIRCULARITEM (*take) (struct circularlist *);
	PCIRCULARITEM  (*takename) (struct circularlist *, char *);
}CIRCULARLIST, *PCIRCULARLIST;

extern PCIRCULARITEM newcircularitem (char *name, void *data, int size);
extern PCIRCULARLIST newcircularlist (char *name);
/*
 * circular list with pointer to data
 */
typedef struct circularitem_ext
{
	CIRCULARITEM  ci; 
}CIRCULARITEM_EXT, *PCIRCULARITEM_EXT;

typedef struct circularlist_ext
{
	CIRCULARLIST cl; 
	/*some functions here*/
}CIRCULARLIST_EXT, *PCIRCULARLIST_EXT;


/**
 * treeitem structure
 * got 1 parent
 * and list of child
 * preparedelete must prepare children for deletion too.
 * have methods : getfirstchild, getnextchild, getparent
 * have variable to hold currentchild
 * tree_item is extended list. with main list 
 * stores properties and children are tree item;
 */
typedef struct tree_item
{ 
	LIST list;/* you can directly perform delete for this item */
	struct tree_item *parent;
	struct tree_item *head;
	struct tree_item *tail;
	struct tree_item *curr;
	struct tree_item *next;
	
	struct tree_item * (*getparent) (struct tree_item *);
	struct tree_item * (*getfirstchild) (struct tree_item *);
	struct tree_item * (*getnextchild) (struct tree_item *); 
	int (*add)(struct tree_item *, struct tree_item *);
	struct tree_item * (*takechild) (struct tree_item *);
	void (*delete) (struct tree_item *);
	struct tree_item * (*detach) (struct tree_item *, struct tree_item *);
	struct tree_item * (*takename) (struct tree_item *, char *);
	struct tree_item * (*getname) (struct tree_item *, char *);
}TREE_ITEM, *PTREE_ITEM;


#endif

