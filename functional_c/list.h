#ifndef _LIST_H_
#define _LIST_H_
#include <any.h>
/**
 * LIST is functional data structure.
 * So it has to have ANY members and methods.
 * It is a single pointer linked list with
 * two reference head and tail
 */
typedef struct list {
	struct list *this;
	/**
	 * NAME			: delete
	 * DESCRIPTION	: delete all nodes from head to last, and delete the list
	 * INPUT
	 *		in		: pointer to itself
	 */
	void (*delete) struct(list*);
	/**
	 * head is the first element of this list
	 */
	ANY *head;
	/**
	 * last it the last element of thist list
	 */
	ANY *last;
	int size;
	/**
	 * NAME			: prepend
	 * DESCRIPTION	: add member to head of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */
	(struct list *) (*prepend) (ANY *); 
	/**
	 * NAME			: append
	 * DESCRIPTION	: add member to end of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */ 
	(struct list *) (*append) (ANY *);
	/**
	 * NAME			: init
	 * DESCRIPTION	: Delete the last element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	(struct list *) (*init) ();
	/**
	 * NAME			: tails
	 * DESCRIPTION	: Delete the first element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	(struct list *) (*tails) ();
	/**
	 * NAME			: get
	 * DESCRIPTION	: get the nth element from head. 
	 *				  the returned element did not detached from
	 *				  the list
	 * INPUT
	 *
	 * RETURNS
	 *	the nth element
	 */ 
	(struct unit *) (*get)(int);
} LIST;
#endif
