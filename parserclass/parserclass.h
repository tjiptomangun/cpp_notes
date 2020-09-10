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

#define PRIMCLASS_PRIMCLASS 0x100 //primitive class
#define PRIMCLASS_PRIMLITEM 0x101 //primitive listitem
#define PRIMCLASS_PRIMLIST  0x102 //primitive list
#define PRIMCLASS_PRIMTREEITEM  0x103 //primitive tree item

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
}STRUCT_CLASS, CLASS, *PSTRUCT_CLASS, *PCLASS; 

extern PCLASS newclass (char *name); 
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
	PL_ITEM    currptr;
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
	char  value[256]; 
	int   (*setvalue) (struct property *, char*);
	int   (*getvalue) (struct property *, char*);
	char *(*getvalue_ptr) (struct property *);
}PROPERTY, *PPROPERTY; 

extern PPROPERTY newproperty (char *name);
extern PPROPERTY newproperty2 (char *name, char *value);

#define MAX_STACKPTR 3000

typedef struct stack_ptr
{
	int top;
	void * c[MAX_STACKPTR];
	int (* init)(struct stack_ptr *);
	int (* push) (struct stack_ptr *, void *);
	void * (* pop) (struct stack_ptr *);
	int (* cleanup) (struct stack_ptr *);
	int (* is_empty) (struct stack_ptr *);
}STACK_PTR, *PSTACK_PTR; 

extern PSTACK_PTR newstackptr ();

/**
 * miniparser is generic file parser structure
 * parse is the real parser function
 */
typedef struct miniparser
{
	FILE *input;
	char *tokenlist;
	int (*parse) (struct miniparser *, PLIST *); 
}MINIPARSER, *PMINIPARSER;

extern PMINIPARSER newminiparser (FILE *in, char *tokenlist, int (*f)(PMINIPARSER, PLIST *));

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

/* **
 *  parallel to class, we need class that do not have any name.
 *  prim means primitive. all primclass derivatives are
 *  for runtime. meanwhile class derivatives are for configurations.
 *  these classes below are mean to provide primitive 
 *  data structure that do not store any data.
 *  the next thing we will do is to make a primitive multinode trees.
 *  this multinode tree is basically hierarchichal lists.
 *  we should able to find an item in the tree. 
 *  we should reduce number of source codes, since it will be hard to 
 *  refactor and make persistent change.
*/

typedef struct primclass 
{
	struct  primclass *this;
	int	type; 
	int (*delete) (struct primclass *);
}STRUCT_PRIMCLASS, PRIMCLASS, *PSTRUCT_PRIMCLASS, *PPRIMCLASS;
 
extern PPRIMCLASS newprimclass (char *name);

typedef struct priml_item
{
	struct primclass primclass;
	struct priml_item *next;
	void * data;
  /*
   * NAME         : set data
   * DESCRIPTION  : default implementation will assign data to this item.
   *                Will return pointer to this instance address if success
   *                , on failure returns NULL.
   *                The only case it is null if data to assign (void) * is null
   * RETURNS
   *	NULL	  : can not set data
   *	Others	  : pointer to this
   **/
	struct priml_item *(*set_data) (struct priml_item *, void *); 
	/**
	 * NAME					: delete
	 * DESCRIPTION	: delete input
	 * RETURNS
	 *		0					: success
	 *	others			: failed, only failed if input is null
	 **/
	int  (*delete) (struct priml_item *); 
	/**
	 * NAME					: get_data
	 * DESCRIPTION	: default implementation will return pointer to data.
	 * RETURNS			: pointer to attached data
	 * 
	 */
	void *(*get_data) (struct priml_item *);
	int (*data_remove_fn) (void *);
	void (*set_data_remove_fn) (struct priml_item *, int (*) (void *));
}PRIML_ITEM, *PPRIML_ITEM; 
extern PPRIML_ITEM newpriml_item ();

typedef struct primlist
{
	PRIML_ITEM priml_item;
	PPRIML_ITEM head;
	PPRIML_ITEM tail;
	PPRIML_ITEM currptr;
	int count;
	PPRIML_ITEM (*take) (struct primlist *);
	PPRIML_ITEM (*get_first_child) (struct primlist *); 
	PPRIML_ITEM (*get_next_child) (struct primlist *); 
	PPRIML_ITEM (*detach) (struct primlist *, PPRIML_ITEM);
	struct primlist *(*set_data) (struct primlist *, void *); 
	int  (*delete) (struct primlist *); 
	void *(*get_data) (struct primlist *); 

	/**
	 * NAME			: add_one
	 * DESCRIPTION		: add an item sorted. If the key already presents then it will replace first match item with item_to_add.
	 * INPUT
	 * 		1st_arg	: the list where item to add
	 * 		2nd_arg	: item to add
	 * 		3rd_arg	: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 				if fnparam1 position before fnparam2 returns value <=-1
	 * 				if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 				if fnparam1 position after fnparam2 returns value >= 1
	 * 				2nd_arg  will be passed as fnparam1.
	 * 				if this functions return 0 then old value will be replaced with this new one.
	 * 				if there is no sorting rule available, just provide with function that returns 1
	 * 				if not match , and 0 if match. So it will always added to tail.
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to add

	 */
	struct primlist * (*add_one ) (struct primlist *, PPRIML_ITEM, int (*) (void *, void *));

	/**
	 * NAME						: remove_one
	 * DESCRIPTION		: delete an item from a sorted list, items are unique so only one item deleted
	 * INPUT
	 * 		1st_arg			: the list where item to add
	 * 		2nd_arg			: item to add
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 									if fnparam1 position after fnparam2 returns value >= 1
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if this functions return 0 then old value will be replaced with this new one.
	 * 									if there is no sorting rule available, just provide with function that returns 1
	 * 									if not match , and 0 if match. So it will always find to end of list.
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to delete
	 */
	struct primlist * (*remove_one ) (struct primlist *, void *, int (*) (void *, void *));
	
	/**
	 * NAME		: add_common
	 * DESCRIPTION	: add item, multiple/common key can coexists
	 * 
	 * 		2nd_arg	: item to add
	 * 		3rd_arg	: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 				if fnparam1 position before fnparam2 returns value <=-1
	 * 				if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 				if fnparam1 position after fnparam2 returns value >= 1
	 * 				2nd_arg  will be passed as fnparam1.
	 * 				if this functions return 0 item to add will be set after previous exists item
	 * 				if there is no sorting new item will be put on tail of list
	 * 				if not match , and 0 if match. So it will always added to tail.
	 * RETURNS
	 * 				1st_arg	: success
	 */
	struct primlist * (*add_common ) (struct primlist *, PPRIML_ITEM, int (*) (void *, void *));
	
	/**
	 * NAME			: remove_common
	 * DESCRIPTION		: delete items from a sorted list, items are not unique, delete multiple item
	 * INPUT
	 * 		1st_arg			: the list where item to add
	 * 		2nd_arg			: item to add
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 									if fnparam1 position after fnparam2 returns value >= 1
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if this functions return 0 then old value will be replaced with this new one.
	 * 									if there is no sorting rule available, just provide with function that returns 1
	 * 									if not match , and 0 if match. So it will always find to end of list.
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to delete
	 */
	struct primlist * (*remove_common ) (struct primlist *, void *, int (*) (void *, void *));
	/**
	 * NAME						: collect
	 * DESCRIPTION		: collect element of list from head to tail
	 * INPUT
	 * 		filter_fn		: function that filter which data will be added into collecting resutl
	 * 									this function should accept data type of list element as input
	 * 									this function should return 1 if condition met, 0 if condition not met
	 * 		collect_fn	: function that add result to collected if filter_fn is true(met)
	 * 									first parameter of this function is element that met filter_fn 
	 * 									the second parameter is current collected values
	 * 									this function do not return anything
	 * 		collected		: is initial condition of collected data. This is the accumator that is used
	 * 									by collect_fn as second parameter
	 * RETURNS				: number of collected item
	 */
	
	/**
	 * NAME						: add_head
	 * DESCRIPTION		: add element to head of this list
	 * INPUT
	 */
	int (*collect) (struct primlist *, int  (*filter_fn) (void *), void (*collect_fn)(void *, void *), void *collected);
	
	/**
	 * NAME						: sort
	 * DESCRIPTION		: reorder item in this list using new comparison function
	 * INPUT
	 * 			argv1			: pointer to this list
	 * 			argv2			: function to compare between to values
	 */
	void (*sort) (struct primlist *, int (*) (void *, void *));
}PRIMLIST, *PPRIMLIST; 

struct tree_item * newtreeitem(struct tree_item *parent, char *name);
PPRIML_ITEM newpriml_item ();
PPRIMLIST newprimlist ();

/**
 * primtree_item structure
 * node
 *    list of attributes
 *    list of child
 * use for this ? Think of xml tree
 */

typedef struct primtree_item
{ 
	PRIMLIST list;/* you can directly perform delete for this item */
	struct primtree_item *parent;
	struct primtree_item *head;
	struct primtree_item *tail;
	struct primtree_item *curr;
	struct primtree_item *next;
	
	struct primtree_item * (*get_parent) (struct primtree_item *);
	struct primtree_item * (*get_first_child) (struct primtree_item *);
	struct primtree_item * (*get_next_child) (struct primtree_item *); 
	int (*add_last )(struct primtree_item *, struct primtree_item *);
	int (*add_first)(struct primtree_item *, struct primtree_item *);
	struct primtree_item * (*detach_head) (struct primtree_item *);
	struct primtree_item * (*detach_node) (struct primtree_item *, struct primtree_item *);
	/**
	 * NAME						: add_one
	 * DESCRIPTION		: add a direct child
	 * INPUT
	 * 		1st_arg			: the tree where item to add
	 * 		2nd_arg			: item to add
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1.
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists).
	 * 									if fnparam1 position after fnparam2 returns value >= 1.
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if this functions return 0 then old value will be replaced with this new one.
	 * 										which means all child will be deleted too.
	 * 									if there is no sorting rule available, just provide with function that returns 1
	 * 									if not match , and 0 if match. So it will always find to end of list.
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to add
	 */
	struct primtree_item * (*add_one ) (struct primtree_item *, struct primtree_item *, int (*) (void *, void *));
	
	/**
	 * NAME						: remove_one
	 * DESCRIPTION		: delete a direct child
	 * INPUT
	 * 		1st_arg			: the tree where item to d
	 * 		2nd_arg			: item to add
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 									if fnparam1 position after fnparam2 returns value >= 1
	 * 									this will only delete one item
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to delete
	 */
	struct primtree_item * (*remove_one ) (struct primtree_item *, void *, int (*) (void *, void *));
	
	
	/**
	 * NAME						: find_one
	 * DESCRIPTION		: find an item from a sorted tree
	 * INPUT
	 * 		1st_arg			: pointer to this
	 * 		2nd_arg			: data to find
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 									if fnparam1 position after fnparam2 returns value >= 1
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if this functions return 0 then old value will be replaced with this new one.
	 * 									if there is no sorting rule available, just provide with function that returns 1
	 * 									if not match , and 0 if match. So it will always find to end of list.
	 * RETURNS
	 * 				NULL		: not found
	 * 				OTHERS	: pointer to child
	 * 				
	 */
	struct primtree_item * (*find_one ) (struct primtree_item *, void *, int (*) (void *, void *));
	
	/**
	 * NAME						: add_common
	 * DESCRIPTION		: add a direct child, sorted. Allow multiple key
	 * INPUT
	 * 		1st_arg			: the tree where item to add
	 * 		2nd_arg			: item to add
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1.
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists).
	 * 									if fnparam1 position after fnparam2 returns value >= 1.
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if this functions return 0 then add this item after that item.
	 * 									if there is no sorting rule available, just provide with function that returns 1
	 * 									if not match , and 0 if match. So it will always find to end of list.
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to add
	 */
	struct primtree_item * (*add_common) (struct primtree_item *, struct primtree_item *, int (*) (void *, void *));
	

	/**
	 * NAME						: remove_common
	 * DESCRIPTION		: delete direct children
	 * INPUT
	 * 		1st_arg			: the tree where item to delete
	 * 		2nd_arg			: item to delete
	 * 		3rd_arg			: a function that accept two arguments (fnparam1 and fnparam2) with condition,
	 * 									if fnparam1 position before fnparam2 returns value <=-1
	 * 									if fnparam1 position exactly in fnparam2 returns 0 (means data already exists)
	 * 									if fnparam1 position after fnparam2 returns value >= 1
	 * 									2nd_arg  will be passed as fnparam1.
	 * 									if multiple value match then all of them removed , as long as data is sorted
	 * RETURNS
	 * 				1st_arg	: success
	 * 				NULL		: failed to delete
	 */
	struct primtree_item * (*remove_common) (struct primtree_item *, void *, int (*) (void *, void *));


	struct primtree_item *(*set_data) (struct primtree_item *, void *); 
	int  (*delete) (struct primtree_item *); 
	void *(*get_data) (struct primtree_item *);
	/**
	 * NAME						: collect
	 * DESCRIPTION		: collect element of tree item recursively with child
	 * INPUT
	 * 		filter_fn		: function that filter which data will be added into collecting resutl
	 * 									this function should accept data type of treeitem data as input
	 * 									this function should return 1 if condition met, 0 if condition not met
	 * 		collect_fn	: function that add result to collected if filter_fn is true(met)
	 * 									first parameter of this function is element that met filter_fn 
	 * 									the second parameter is current collected values
	 * 									this function do not return anything
	 * 		collected		: is initial condition of collected data. This is the accumator that is used
	 * 									by collect_fn as second parameter
	 * RETURNS				: number of collected item
	 * 									
	 * 									
	 */
	int (*collect) (struct primtree_item *, int  (*filter_fn) (void *), void (*collect_fn)(void *, void *), void *collected);
	
	/**
	 * NAME						: sort
	 * DESCRIPTION		: reorder direct child of this item using new comparison function
	 * INPUT
	 * 			argv1			: pointer to this list
	 * 			argv2			: function to compare between to values
	 */
	void (*sort) (struct primtree_item *, int (*) (void *, void *));
}PRIMTREE_ITEM, *PPRIMTREE_ITEM;

#endif

