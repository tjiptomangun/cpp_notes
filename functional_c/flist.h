#ifndef _FLIST_H_
#define _FLIST_H_
#include <any.h>
#include <option.h>

/**
 * LIST_NODE is helper struct for list member
 * it is not a functional data structure but
 * wrap functional data structure.
 * It's constructor and destructor are implemented
 * on LIST
 */
typedef struct flist_node{
	struct flist_node *next;
	struct flist_node *prev; 
	ANY *wrapped_data;
} FLIST_NODE;
/**
 * LIST is functional data structure.
 * So it has to have ANY members and methods.
 * It is a single pointer linked list with
 * two reference __s__head and tail
 */
typedef struct flist {
	struct flist *this;
	/**
	 * NAME			: delete
	 * DESCRIPTION	: delete all nodes from __s__head to __s__last, and delete the list
	 * INPUT
	 *		in		: pointer to itself
	 */
	void (*delete) (struct flist*);

	/**
	 * NAME			: copy
	 * DESCRIPTION	: create a copy of this list
	 */
	struct flist * (*copy) (struct flist *);
	/**
	 * __s__head is the first element of this list
	 */
	FLIST_NODE *__s__head;
	/**
	 * __s__last it the __s__last element of thist list
	 */
	FLIST_NODE *__s__last;
	int size;
	/**
	 * NAME			: prepend
	 * DESCRIPTION	: add member to __s__head of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */
	struct flist * (*prepend) (struct flist*, ANY *); 
	/**
	 * NAME			: append
	 * DESCRIPTION	: add member to end of list
	 * INPUT
	 *				: node to add
	 * RETURNS
	 *	This list with a node added
	 */ 
	struct flist * (*append) (struct flist*, ANY *);
	/**
	 * NAME			: init
	 * DESCRIPTION	: Delete the __s__last element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	struct flist * (*init) (struct flist*);
	/**
	 * NAME			: tail
	 * DESCRIPTION	: Delete the first element.
	 * INPUT
	 *
	 * RETURNS
	 *	The new list
	 */ 
	struct flist * (*tail) (struct flist*);
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
	ANY * (*get)(struct flist*, unsigned int);

	/**
	 * NAME			: head
	 * DESCRIPTION	: get the first element. This is equal to get(0)
	 *
	 */
	ANY * (*head)(struct flist*);

	/**
	 * NAME			: last
	 * DESCRIPTION	: get the last element
	 *
	 */
	ANY * (*last)(struct flist*);

	/**
	 * NAME			: fold_left
	 * DESCRIPTION	: iterate this list and apply function fn from left.
	 *				 
	 * INPUT
	 *		inlist	: the list to fold
	 *		acc		: accumulating value
	 *		fn		: function which returns value of type acc
	 * fn param1	: accumulator
	 * fn param2	: current list item value	
	 */ 
	ANY * (*fold_left)(struct flist* inlist, ANY *acc, ANY *(*fn)(ANY *, ANY *));

	/**
	 * NAME			: fold_right
	 * DESCRIPTION	: iterate this list and apply function fn from right.
	 *				  
	 * INPUT
	 *		inlist	: the list to fold
	 *		acc		: accumulating value
	 *		fn		: function which returns value of type acc
	 * fn param1	: accumulator
	 * fn param2	: current list item value	
	 */ 
	ANY * (*fold_right)(struct flist* inlist, ANY *acc, ANY *(*fn)(ANY *, ANY *));

	/**
	 * NAME			: flip
	 * DESCRIPTION	: reverse list nodes postition. In place.
	 * INPUT
	 *		inlist	: the list to flip, mutated
	 * RETURN		: pointer to this list after reversed
	 *		
	 */
	void (*flip)(struct flist *inlist);

	/**
	 * NAME			: reverse
	 * DESCRIPTION	: new list with reversed postition.
	 * INPUT
	 *		inlist	: the list to reverse
	 * RETURN		: pointer to new list 
	 *		
	 */
	struct flist* (*reverse)(struct flist *inlist);

	
	/**
	 * NAME			: map
	 * DESCRIPTION	: change a list of type x to list of type y
	 * INPUT
	 *		inlist	: list to tranform
	 *		fn		: function with parameter of inlist member type
	 *				  and return of desired output list member type
	 *				  example list of type integer to list of type string
	 * RETURNS		: new tranformed list. inlist is not mutated.
	 */
	struct flist* (*map)(struct flist *inlist, ANY* (*fn)(ANY *));


	/**
	 * NAME			: filter
	 * DESCRIPTION	: create a new list that match fn condition
	 * INPUT
	 *		inlist	: list to filter
	 *		fn		: function which accept inlist member type
	 *				  and return boolean *				  
	 * RETURNS		: new tranformed list. inlist is not mutated.
	 */
	struct flist* (*filter)(struct flist *inlist, int (*fn)(ANY *));

	/**
	 * NAME			: collect
	 * DESCRIPTION	: create a new list that filter and tranform input
	 * INPUT
	 *		inlist	: list to filter and transfor
	 *		fn		: function which accept inlist member type
	 *				  and return option object of filtered and transform.
	 *				  if fn return none_object then it will not be added
	 *				  to new list. If it is some_object then added
	 * RETURNS		: new tranformed list. inlist is not mutated.
	 */
	struct flist* (*collect)(struct flist *inlist, OPTION *(*fn)(ANY *));

	/**	
	 * NAME			: zip
	 * DESCRIPTION	: create a new list with its members are 2-tuple of each original list element
	 * INPUT
	 *		inlist	: list to zip with
	 *		otherlist : the second list to zip with
	 * 
	 */
	struct flist* (*zip) (struct flist *inlist, struct flist *otherlist);

	/**
	 * NAME			: take
	 * DESCRIPTION	: take first n member
	 * INPUT
	 *		inlist  : to take
	 *		n		: number of member to take
	 */
	struct flist* (*take) (struct flist *inlist, int n);

} FLIST;

FLIST *new_list();

/**
 *list case class 
*/
typedef struct Cons{
	ANY *hd;
	FLIST *tl;
} CONS;

/**
 * NAME			: uncons
 * DESCRIPTION	: destructure list by separating head with tail
 */
CONS *uncons(FLIST *in);

/**
 * NAME			: free_cons
 * DESCRIPTION	: helper to free list construct. 
 *				  head and tail data will not be freed.
 */
void free_cons(CONS *cons);

/**
 * NAME			: list_create
 * DESCRIPTION	: create list with its ANY* items
 */
FLIST *list_create(int num_items, ...);
#endif
