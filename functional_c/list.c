#include <list.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <any.h>

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
 * DESCRIPTION	: assign new data as inlist __s__head
 * INPUT
 *		inlist	: list to contain the indata
 *		indata	: data to fill the list
 *	RETURNS
 *		the inlist
 */
static LIST *__prepend_list(LIST*inlist, ANY *indata) {
	if (inlist && indata){
		LIST_NODE *new_node = __new_list_node(indata, NULL, inlist->__s__head);
		inlist->__s__head = new_node;
		if (inlist->__s__last == NULL)
			inlist->__s__last = new_node;
		inlist->size ++;
	}
	return inlist; 
}

/**
 * NAME			: __append_list
 * DESCRIPTION	: assign new data as inlist __s__last
 * INPUT
 *		inlist	: list to contain the indata
 *		indata	: data to fill the list
 *	RETURNS
 *		the inlist
 */
static LIST *__append_list(LIST*inlist, ANY *indata) {
	if(inlist && indata){
		LIST_NODE *new_node = __new_list_node(indata, inlist->__s__last, NULL);
		inlist->__s__last = new_node;
		if (inlist->__s__head == NULL)
			inlist->__s__head = new_node;
		inlist->size ++;
	}
	return inlist;
}
/**
 * NAME			: __init_list
 * DESCRIPTION	: delete the __s__last element of given list
 */
static LIST *__init_list(LIST *inlist) {
	LIST_NODE *temp;
	if (inlist && inlist->__s__last){
		temp = inlist->__s__last->prev;
		__delete_list_node(inlist->__s__last);
		if (inlist->__s__head == inlist->__s__last)
			inlist->__s__head = temp;
		inlist->__s__last = temp;
		inlist->size --;
	}
	return inlist;
}

/**
 * NAME			: __tail_list
 * DESCRIPTION	: delete first element of given list
 */
static LIST *__tail_list(LIST *inlist){
	LIST_NODE *temp;
	if(inlist && inlist->__s__head){
		temp = inlist->__s__head->next;
		__delete_list_node(inlist->__s__head);
		if (inlist->__s__head == inlist->__s__last)
			inlist->__s__last = temp;
		inlist->__s__head = temp;
		inlist->size --; 
	}
	return inlist;
}

/**
 * NAME			: __delete_list
 * DESCRIPTION	: delete all list node and list node data (using __init_list).
 *				  free this list
 */
static void __delete_list(LIST *inlist) { 
	while(inlist && inlist->__s__head){
		__init_list(inlist);
	}
	inlist->this = NULL;
	inlist->delete = NULL;
	inlist->prepend = NULL;
	inlist->append = NULL;
	inlist->init = NULL;
	inlist->tail = NULL;
	inlist->get = NULL;
	inlist->head = NULL;
	inlist->last = NULL;
	free(inlist);
}

static ANY * __get_nth(LIST *inlist, unsigned int ndx) {
	int i = 0;
	LIST_NODE *curr = inlist->__s__head;
	if (ndx >= inlist->size)	
		return NULL;
	while(i < ndx){
		curr = curr->next;	
		i++;
	}
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data;
}

static ANY * __head(LIST *inlist) {
	LIST_NODE *curr = inlist->__s__head;
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data;
}

static ANY * __last(LIST *inlist) {
	LIST_NODE *curr = inlist->__s__last;
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data;
}

LIST *new_list() {
	LIST *ret = (LIST *) calloc(1, sizeof(LIST));
	ret->this = ret;
	ret->delete = __delete_list; 
	ret->__s__head = NULL;
	ret->__s__last = NULL;
	ret->size = 0;
	ret->prepend = __prepend_list;
	ret->append = __append_list;
	ret->init = __init_list;
	ret->tail = __tail_list;
	ret->get = __get_nth;
	ret->head = __head;
	ret->last= __last;
	return ret;
}

#ifdef _LIST_UNIT_TEST_
	void create_list(){
		printf("-create list\n");
		LIST *t = new_list();
		assert(t != NULL);
		t->delete(t);
	}

	void prepend_list(){
		printf("-prepend list\n");
		LIST *t = new_list();
		unsigned char *h1, *t1;
		unsigned char *h2, *t2;
		assert(t != NULL);
		t->prepend(t, new_any());
		h1 = (unsigned char *)t->__s__head;
		t1 = (unsigned char *)t->__s__last;
		t->prepend(t, new_any());
		h2 = (unsigned char *)t->__s__head;
		t2 = (unsigned char *)t->__s__last;
		assert(t->size == 2);
		assert(t->__s__head != NULL);
		assert(t->__s__head->next != NULL);
		assert(h1 != h2);
		assert(t1 == t2);
		t->delete(t);
	}

	void append_list(){
		printf("-append list\n");
		LIST *t = new_list();
		unsigned char *h1, *t1;
		unsigned char *h2, *t2;
		assert(t != NULL);
		t->append(t, new_any());
		h1 = (unsigned char *)t->__s__head;
		t1 = (unsigned char *)t->__s__last;
		t->append(t, new_any());
		h2 = (unsigned char *)t->__s__head;
		t2 = (unsigned char *)t->__s__last;
		assert(t->size == 2);
		assert(t->__s__head != NULL);
		assert(t->__s__head->next != NULL);
		assert(h1 == h2);
		assert(t1 != t2);
		t->delete(t);
	}

	void init_list(){
		printf("-init list\n");
		LIST *t = new_list();
		unsigned char  *t1;
		unsigned char  *t2;
		unsigned char  *t3;
		assert(t != NULL);
		t->append(t, new_any());
		t1 = (unsigned char *)t->__s__last;
		t->append(t, new_any());
		t2 = (unsigned char *)t->__s__last;
		t->append(t, new_any());
		t3 = (unsigned char *)t->__s__last;
		assert(t->size == 3);
		assert((unsigned char *)t->__s__last == t3); 
		t->init(t);
		assert((unsigned char *)t->__s__last == t2);
		t->init(t);
		assert((unsigned char *)t->__s__last == t1);
		assert(t->size == 1); 
		t->delete(t);
	}

	void tail_list(){
		printf("-tail list\n");
		LIST *t = new_list();
		unsigned char *t1;
		unsigned char *t2;
		unsigned char *t3;
		assert(t != NULL);
		t->append(t, new_any());
		t1 = (unsigned char *)t->__s__last;
		t->append(t, new_any());
		t2 = (unsigned char *)t->__s__last;
		t->append(t, new_any());
		t3 = (unsigned char *)t->__s__last;
		assert(t->size == 3);
		assert((unsigned char *)t->__s__head== t1); 
		t->tail(t);
		assert((unsigned char *)t->__s__head== t2);
		t->tail(t);
		assert((unsigned char *)t->__s__head== t3);
		assert(t->size == 1); 
		t->delete(t);
	}
	void get(){
		printf("-get\n");
		LIST *t = new_list();
		LIST_NODE *t2;
		LIST_NODE *t3;
		LIST_NODE *t4;
		assert(t != NULL);
		t->append(t, new_any());
		t->append(t, new_any());
		t2 = t->__s__last;
		t->append(t, new_any());
		t3 = t->__s__last;
		t->append(t, new_any());
		t4 = t->__s__last;
		assert(t->get(t, 3) == t4->wrapped_data); 
		t->tail(t);
		assert(t->get(t, 2) == t4->wrapped_data);
		assert(t->get(t, 1) == t3->wrapped_data);
		assert(t->get(t, 0) == t2->wrapped_data);
		t->init(t);
		t->delete(t);
	}
	void head_last(){
		printf("-head_last\n");
		LIST *t = new_list();
		LIST_NODE *t1;
		LIST_NODE *t2;
		LIST_NODE *t3;
		LIST_NODE *t4;
		assert(t != NULL);
		t->append(t, new_any());
		t1 = t->__s__last;
		t->append(t, new_any());
		t2 = t->__s__last;
		t->append(t, new_any());
		t3 = t->__s__last;
		t->append(t, new_any());
		t4 = t->__s__last;
		assert(t->last(t) == t4->wrapped_data); 
		assert(t->head(t) == t1->wrapped_data); 
		t->tail(t);
		assert(t->head(t) == t2->wrapped_data); 
		t->init(t);
		assert(t->last(t) == t3->wrapped_data); 
		t->delete(t);
	}

	int main (int argc, char **argv) {
		create_list();
		prepend_list();
		append_list();
		init_list();
		tail_list();
		get();
		head_last();
	}
#endif
