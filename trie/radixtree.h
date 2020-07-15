/**
 * author : henky <hanky.acbb@telogic.com.sg>
 */
#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_
typedef struct radixtree_item{
char name[20];
int name_len;
struct radixtree_item *parent;
struct radixtree_item *head;
struct radixtree_item *tail;
struct radixtree_item *next; 
int is_word;
}RADIXTREE_ITEM, *PRADIXTREE_ITEM;

// /**
//  * NAME						: radixtreeitem_add 
//  * DESCRIPTION		: add to tail of a parent node
//  * INPUT
//  * 		root				: parent of the new node
//  * 		addedchild	: the added node
//  */
// PRADIXTREE_ITEM radixtreeitem_add (struct radixtree_item *parent, struct radixtree_item *addedchild);

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
PRADIXTREE_ITEM radixtreeitem_insertkey(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME						: find_name
 * DESCRIPTION		: find a desendant with such key
 * INPUT
 * 				parent	: parent of the key
 * 				name		: key to find
 */
PRADIXTREE_ITEM radixtreeitem_findkey(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: find_prefix
 * DESCRIPTION	: find if a string prefix meet any of key of a parent
 * INPUT
 * 			parent	: parent to keys
 * 			name		: string to find
 *
 */
PRADIXTREE_ITEM radixtreeitem_findprefix(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: delete_node
 * DESCRIPTION	: delete a node and its child. Function does not manage how it's
 * 								link to next of head/tail of its parent. It is caller that should
 * 								manage.
 * INPUT
 * 			root		: node to delete
 * 
 */
int radixtreeitem_deletenode(PRADIXTREE_ITEM node);

/**
 * NAME					: delete_key
 * DESCRIPTION	: delete a key from tree
 * INPUT
 * 			parent	: tree that have this key
 * 			name		: key to delete
 */
void radixtreeitem_deletekey(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: print_tree
 */
void print_tree(PRADIXTREE_ITEM root, int tab_count);

PRADIXTREE_ITEM new_radixtreeitem(char *name, int name_len, PRADIXTREE_ITEM parent);

/**
 * NAME 				: radixtreeitem_getkeywords
 * DESCRIPTION	: get keywords in this tree and store in bufout with each key delimited by delim
 * OUTPUT
 * 		bufout		: output storage
 * INPUT
 * 		bufout_max: maximum string can be copied minus bufout_count
 * 		bufcount	: num char already copied
 * 		delim			: delimiters
 * 		
 */
void radixtreeitem_getkeywords(PRADIXTREE_ITEM item,  char *bufout, int bufout_max, int *bufout_count, char *delim);

void radixtreeitem_getkeyreverse(PRADIXTREE_ITEM item, char *bufout, int bufout_max, int *bufout_count);
#endif
