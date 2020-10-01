/**
 * author : henky <hanky.acbb@telogic.com.sg>
 *          2020/09/14
 *          rdxtree is radix like radixtree without parent pointer
 *          radix tree data structure algorithm do not need parent pointer at all
 */
#ifndef _RDXTREE_H_
#define _RDXTREE_H_
typedef struct rdxtree_item{
char name[20];
int name_len;
struct rdxtree_item *head;
struct rdxtree_item *tail;
struct rdxtree_item *next; 
int is_word;
}RDXTREE_ITEM, *PRDXTREE_ITEM;

typedef struct rdxtree_context_item{
struct rdxtree_item *item; 
struct rdxtree_context_item *next;
}RDXTREE_CONTEXT_ITEM, *PRDXTREE_CONTEXT_ITEM;


typedef struct rdxtree_find_context{
	PRDXTREE_CONTEXT_ITEM head;
	PRDXTREE_CONTEXT_ITEM tail;
	int reversed;
	struct rdxtree_find_context *(*add_item)(struct rdxtree_find_context *, PRDXTREE_CONTEXT_ITEM);
	struct rdxtree_find_context *(*reverse_once)(struct rdxtree_find_context *);
	void (*delete)(struct rdxtree_find_context *);
}RDXTREE_FIND_CONTEXT, *PRDXTREE_FIND_CONTEXT;

typedef struct rdxtree_stack_context{
	PRDXTREE_CONTEXT_ITEM top;
	PRDXTREE_CONTEXT_ITEM (*push)(struct rdxtree_stack_context *, PRDXTREE_CONTEXT_ITEM);
	PRDXTREE_CONTEXT_ITEM (*pop)(struct rdxtree_stack_context *);
	void (*delete)(struct rdxtree_stack_context *);
}RDXTREE_STACK_CONTEXT, *PRDXTREE_STACK_CONTEXT;

/**
 * NAME						: insert_child
 * DESCRIPTION		: add descendants key to a parent
 * INPUT
 * 			parent		: parent ascendant of the new key
 * 			name			: the key
 * RETURNS
 * 			NULL			: cannot allocate memory
 * 			OTHERS		: pointer to newly added key
 */
PRDXTREE_ITEM rdxtreeitem_insertkey(PRDXTREE_ITEM parent, char *name);

/**
 * NAME						: find_name
 * DESCRIPTION		: find a desendant with such key
 * INPUT
 * 				parent	: parent of the key
 * 				name		: key to find
 * 				ctx			: is in out context that will populates with pointers from 
 * 									parent to returned result. See RDXTREE_FIND_CONTEXT for details
 * RETURNS				:
 * 			NULL			: Not found
 * 			OTHERS		: Pointer to final result
 */
PRDXTREE_ITEM rdxtreeitem_findkey(PRDXTREE_ITEM parent, char *name, PRDXTREE_FIND_CONTEXT ctx);

/**
 * NAME					: find_prefix
 * DESCRIPTION	: find if a string prefix meet any of key of a parent
 * INPUT
 * 			parent	: parent to keys
 * 			name		: string to find
 * 			ctx			: is in out context that will populates with pointers from 
 * 								parent to returned result. See RDXTREE_FIND_CONTEXT for details
 * RETURNS			:
 * 			NULL		: Not found
 * 			OTHERS	: Pointer to final result
 */
PRDXTREE_ITEM rdxtreeitem_findprefix(PRDXTREE_ITEM parent, char *name, PRDXTREE_FIND_CONTEXT);

/**
 * NAME					: delete_node
 * DESCRIPTION	: delete a node and its child. Function does not manage how it's
 * 								link to next of head/tail of its parent. It is caller that should
 * 								manage.
 * INPUT
 * 			root		: node to delete
 * 
 */
int rdxtreeitem_deletenode(PRDXTREE_ITEM node);

/**
 * NAME					: delete_key
 * DESCRIPTION	: delete a key from tree
 * INPUT
 * 			parent	: tree that have this key
 * 			name		: key to delete
 */
void rdxtreeitem_deletekey(PRDXTREE_ITEM parent, char *name);

/**
 * NAME					: print_tree
 */
void print_rdx_tree(PRDXTREE_ITEM root, int tab_count);

PRDXTREE_ITEM new_rdxtreeitem(char *name, int name_len);

/**
 * NAME 				: rdxtreeitem_getkeywords
 * DESCRIPTION	: get keywords in this tree and store in bufout with each key delimited by delim
 * OUTPUT
 * 		bufout		: output storage
 * INPUT
 * 		bufout_max: maximum string can be copied minus bufout_count
 * 		bufcount	: num char already copied
 * 		delim			: delimiters
 * 		
 */
void rdxtreeitem_getkeywords(PRDXTREE_ITEM item,  char *bufout, int bufout_max, int *bufout_count, char *delim);

/**
 * NAME					: new_rdxtree_find_context
 * DESCRIPTION	: create new rdx tree find context
 */
PRDXTREE_FIND_CONTEXT new_rdxtree_find_context(void);

/**
 * NAME					: new_rdxtree_find_context
 * DESCRIPTION	: create new rdx tree stack context
 */
PRDXTREE_STACK_CONTEXT new_rdxtree_stack_context();

/**
 * NAME					: rdxtreeitem_ctor
 * DESCRIPTION	: rdxtreeitem constructor routine. Use this
 * 								if RDXTREE_ITEM is created without new_rdxtreeitem
 */
void rdxtreeitem_ctor(PRDXTREE_ITEM new_item, char *name, int name_len);

/**
 * NAME					: rdxtree_find_context_ctor
 * DESCRIPTION	: rdxtree_find_context constructor routing. Use this
 * 								if RDXTREE_FIND_CONTEXT is created without new_rdxtree_stack_context
 */
PRDXTREE_FIND_CONTEXT rdxtree_find_context_ctor(PRDXTREE_FIND_CONTEXT ctx);


#endif
