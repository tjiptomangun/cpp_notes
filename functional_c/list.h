#ifndef _LIST_H_
#define _LIST_H_
#include <any.h>

/**
 * LIST_NODE is helper struct for list member
 * it is not a functional data structure but
 * wrap functional data structure.
 * It's constructor and destructor are implemented
 * on LIST
 */
typedef struct list_node{
	struct list_node *next;
	struct list_node *prev; 
	ANY *wrapped_data;
} LIST_NODE;
/**
 * LIST is functional data structure.
 * So it has to have ANY members and methods.
 * It is a single pointer linked list with
 * two reference __s__head and tail
 */
typedef struct list {
	struct list *this;
	/**
	 * NAME			: delete
	 * DESCRIPTION	: delete all nodes from __s__head to __s__last, and delete the list
	 * INPUT
	 *		in		: pointer to itself
	 */
	void (*delete) (struct list*);
	/**
	 * __s__head is the first element of this list
	 */
	LIST_NODE *__s__head;
	/**
	 * __s__last it the __s__last element of thist list
	 */
	LIST_NODE *__s__last;
	int size;
	/**
	 * NAME			: prepend
	 * DESCRIPTION	: add member to __s__head of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */
	struct list * (*prepend) (struct list*, ANY *); 
	/**
	 * NAME			: append
	 * DESCRIPTION	: add member to end of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */ 
	struct list * (*append) (struct list*, ANY *);
	/**
	 * NAME			: init
	 * DESCRIPTION	: Delete the __s__last element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	struct list * (*init) (struct list*);
	/**
	 * NAME			: tail
	 * DESCRIPTION	: Delete the first element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	struct list * (*tail) (struct list*);
	/**
	 * NAME			: get
	 * DESCRIPTION	: get the nth element from __s__head. 
	 *				  the returned element did not detached from
	 *				  the list.
	 * INPUT
	 *			n	: the index
	 *
	 * RETURNS
	 *	the nth element
	 */ 
	ANY * (*get)(struct list*, unsigned int);

	/**
	 * NAME			: head
	 * DESCRIPTION	: get the first element. This is equal to get(0)
	 *
	 */
	ANY * (*head)(struct list*);

	/**
	 * NAME			: last
	 * DESCRIPTION	: get the last element
	 *
	 */
	ANY * (*last)(struct list*);


} LIST;
#endif