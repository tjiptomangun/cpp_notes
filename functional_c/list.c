#include <list.h>
#include <stdlib.h>
#include <assert.h>

/**
 * NAME			: __new_list_node
 * DESCRIPTION	: create a new list node
 * INPUT
 *		prev	: the predecessor of this new node
 *		next	: the successor of this new node
 *		wrapped	: the data wrapped by this node
 */
static LIST_NODE * __new_list_node(ANY* wrapped, LIST_NODE *prev, LIST_NODE *next) {
	if (wrapped){
		LIST_NODE *ret = (LIST_NODE *) calloc (1, sizeof (LIST_NODE));
		if (wrapped != NULL)
			ret->wrapped_data = wrapped;
		if (prev) {
			prev->next =  ret;
			ret->prev = prev;
		}
		if (next) {
			next->prev = ret;
			ret->next = next;
		}
		return ret;
	}
	else
		return NULL;
	
}
/**
 * NAME			: __delete_list_node
 * DESCRIPTION	:delete the wrapped data, detach from other member
 */
static void __delete_list_node(LIST_NODE *node) {
	if (node){
		if (node->wrapped_data){
			node->wrapped_data->delete(node->wrapped_data);
			node->wrapped_data = NULL;
		}
		if (node->prev){
			node->prev->next = node->next;
			node->next = NULL;
		}
		if (node->next) {
			node->next->prev = node->prev;
			node->prev = NULL;
		}
		free(node);
	}
}
/**
 * NAME			: __prepend_list
 * DESCRIPTION	: assign new data as inlist head
 * INPUT
 *		inlist	: list to contain the indata
 *		indata	: data to fill the list
 *	RETURNS
 *		the inlist
 */
static LIST *__prepend_list(LIST*inlist, ANY *indata) {
	if (inlist && indata){
		LIST_NODE *new_node = __new_list_node(indata, NULL, inlist->head);
		inlist->head = new_node;
		if (inlist->last == NULL)
			inlist->last = new_node;
		inlist->size ++;
	}
	return inlist; 
}

/**
 * NAME			: __append_list
 * DESCRIPTION	: assign new data as inlist last
 * INPUT
 *		inlist	: list to contain the indata
 *		indata	: data to fill the list
 *	RETURNS
 *		the inlist
 */
static LIST *__append_list(LIST*inlist, ANY *indata) {
	if(inlist && indata){
		LIST_NODE *new_node = __new_list_node(indata, inlist->last, NULL);
		inlist->last = new_node;
		if (inlist->head == NULL)
			inlist->head = new_node;
		inlist->size ++;
	}
	return inlist;
}
/**
 * NAME			: __init_list
 * DESCRIPTION	: delete the last element of given list
 */
static LIST *__init_list(LIST *inlist) {
	LIST_NODE *temp;
	if (inlist && inlist->last){
		temp = inlist->last->prev;
		__delete_list_node(inlist->last);
		inlist->last = temp;
		inlist->size --;
	}
	return inlist;
}

/**
 * NAME			: __tails_list
 * DESCRIPTION	: delete first element of given list
 */
static LIST *__tails_list(LIST *inlist){
	LIST_NODE *temp;
	if(inlist && inlist->head){
		temp = inlist->head->next;
		__delete_list_node(inlist->head);
		inlist->head = temp;
		inlist->size --; 
	}
	return inlist;
}

static void __delete_list(LIST *inlist) { 
	while(inlist && inlist->head){
		__init_list(inlist);
	}
	inlist->this = NULL;
	inlist->delete = NULL;
	inlist->prepend = NULL;
	inlist->append = NULL;
	inlist->init = NULL;
	inlist->tails = NULL;
	free(inlist);
}
LIST *new_list() {
	LIST *ret = (LIST *) calloc(1, sizeof(LIST));
	ret->this = ret;
	ret->delete = __delete_list; 
	ret->head = NULL;
	ret->last = NULL;
	ret->size = 0;
	ret->prepend = __prepend_list;
	ret->append = __append_list;
	ret->init = __init_list;
	ret->tails = __tails_list;
	return ret;
}

#ifdef _LIST_UNIT_TEST_
	void create_list(){
		LIST *t = new_list();
		assert(t != NULL);
		t->delete(t);
	}	
	int main (int argc, char **argv) {
		create_list();
	}
#endif
