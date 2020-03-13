#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_
typedef struct radixtree_item{
char name[16];
int name_len;
struct radixtree_item *parent;
struct radixtree_item *head;
struct radixtree_item *tail;
struct radixtree_item *next; 
int is_word;
}RADIXTREE_ITEM, *PRADIXTREE_ITEM;

/**
 * NAME						: radixtreeitem_add 
 * DESCRIPTION		: add to tail of a parent node
 * INPUT
 * 		root				: parent of the new node
 * 		addedchild	: the added node
 */
PRADIXTREE_ITEM radixtreeitem_add (struct radixtree_item *parent, struct radixtree_item *addedchild);

/**
 * NAME						: insert_child
 * DESCRIPTION		: add descendants key to a parent
 * INPUT
 * 			parent		: parent ascendant of the new key
 * 			name			: the key
 * 
 */
PRADIXTREE_ITEM insert_child(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME						: find_name
 * DESCRIPTION		: find a desendant with such key
 * INPUT
 * 				parent	: parent of the key
 * 				name		: key to find
 */
PRADIXTREE_ITEM find_name(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: find_prefix
 * DESCRIPTION	: find if a string prefix meet any of key of a parent
 * INPUT
 * 			parent	: parent to keys
 * 			name		: string to find
 *
 */
PRADIXTREE_ITEM find_prefix(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: delete_node
 * DESCRIPTION	: delete a node and its child. Function does not manage how it's
 * 								link to next of head/tail of its parent. It is caller that should
 * 								manage.
 * INPUT
 * 			root		: node to delete
 * 
 */
int delete_node(PRADIXTREE_ITEM node);

/**
 * NAME					: delete_key
 * DESCRIPTION	: delete a key from tree
 * INPUT
 * 			parent	: tree that have this key
 * 			name		: key to delete
 */
void delete_key(PRADIXTREE_ITEM parent, char *name);

/**
 * NAME					: print_tree
 */
void print_tree(PRADIXTREE_ITEM root, int tab_count);

PRADIXTREE_ITEM new_radixtreeitem(char *name, int name_len, PRADIXTREE_ITEM parent);
#endif
