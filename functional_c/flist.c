#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <stdarg.h> 
#include "flist.h"
#include "any.h"
#include "tuple.h"

/**
 * NAME			: __new_list_node
 * DESCRIPTION	: create a new list node
 * INPUT
 *		prev	: the predecessor of this new node
 *		next	: the successor of this new node
 *		wrapped	: the data wrapped by this node
 */
static FLIST_NODE* __new_list_node(ANY* wrapped, FLIST_NODE *prev, FLIST_NODE *next) {
	if (wrapped){
		FLIST_NODE *ret = (FLIST_NODE *) calloc (1, sizeof (FLIST_NODE));
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
 * DESCRIPTION	: delete the wrapped data, detach from other member
 */
static void __delete_list_node(FLIST *ls, FLIST_NODE *node) {
	if (node){
		if (node->wrapped_data){
			node->wrapped_data->delete(node->wrapped_data);
			node->wrapped_data = NULL;
		}
		if (ls->__s__head == node){
			ls->__s__head = node->next;
		}
		if (ls->__s__last == node){
			ls->__s__last = node->prev;
		}
		if (node->prev){
			node->prev->next = node->next;
			node->next = NULL;
		}
		if (node->next) {
			node->next->prev = node->prev;
			node->prev = NULL;
		}
		ls->size --;
		free(node);
	}
}

/**
 * NAME			: __detach_list_node
 * DESCRIPTION	: detach from other member
 */
/*
static FLIST_NODE* __detach_list_node(FLIST_NODE *node) {
	if (node){
		if (node->prev){
			node->prev->next = node->next;
			node->next = NULL;
		}
		if (node->next) {
			node->next->prev = node->prev;
			node->prev = NULL;
		} 
	}
	return node;
}
*/

/**
 * NAME			: __release_wrapped_data
 * DESCRIPTION	: release wrapped_data
 */
static ANY* __release_wrapped_data(FLIST_NODE *node) {
	ANY *ret = NULL;
	if (node && node->wrapped_data){
		ret = node->wrapped_data;
		node->wrapped_data = NULL;
		return ret; 
	}
	else{
		return NULL;
	}

}

/**
 * NAME			: __release_wrapped_and_delete
 * DESCRIPTION	: release wrapped data in a node and delete the node
 */
static ANY* __release_wrapped_and_delete(FLIST *ls, FLIST_NODE *node){
	ANY *ret = __release_wrapped_data(node);
	__delete_list_node(ls, node);
	return ret;
}

static FLIST *__list_release_all_wrapped(FLIST *ls) {
	FLIST_NODE *curr = ls->__s__head;
	while(curr){
		curr->wrapped_data = NULL;
		curr = curr->next;
	}
	return ls;
}

/**
 * NAME			: __unwrap_list_node
 * DESCRIPTION	: return wrapped data
 */
/*
static ANY *__unwrap_list_node(FLIST_NODE *node){
	if(node && node->wrapped_data)
		return node->wrapped_data;

	return NULL;
}
*/

/**
 * NAME			: __prepend_list
 * DESCRIPTION	: assign new data as inlist __s__head
 * INPUT
 *		inlist	: list to contain the indata
 *		indata	: data to fill the list
 *	RETURNS
 *		the inlist
 */
static FLIST *__prepend_list(FLIST*inlist, ANY *indata) {
	if (inlist && indata){
		FLIST_NODE *new_node = __new_list_node(indata, NULL, inlist->__s__head);
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
static FLIST *__append_list(FLIST*inlist, ANY *indata) {
	if(inlist && indata){
		FLIST_NODE *new_node = __new_list_node(indata, inlist->__s__last, NULL);
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
static FLIST *__init_list(FLIST *inlist) {
	FLIST_NODE *temp;
	if (inlist && inlist->__s__last){
		temp = inlist->__s__last->prev;
		__delete_list_node(inlist, inlist->__s__last);
		if (inlist->__s__head == inlist->__s__last)
			inlist->__s__head = temp;
		inlist->__s__last = temp;
	}
	return inlist;
}

/**
 * NAME			: __tail_list
 * DESCRIPTION	: delete first element of given list
 */
static FLIST *__tail_list(FLIST *inlist){
	FLIST_NODE *temp;
	if(inlist && inlist->__s__head){
		temp = inlist->__s__head->next;
		__delete_list_node(inlist, inlist->__s__head);
		if (inlist->__s__head == inlist->__s__last)
			inlist->__s__last = temp;
		inlist->__s__head = temp;
	}
	return inlist;
}

/**
 * NAME			: __delete_list
 * DESCRIPTION	: delete all list node and list node data (using __init_list).
 *				  free this list
 */
static void __delete_list(FLIST *inlist) { 
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
	inlist->fold_left = NULL;
	inlist->fold_right = NULL;
	inlist->flip = NULL;
	inlist->reverse = NULL;
	inlist->map = NULL;
	inlist->delete = NULL;
	inlist->collect = NULL;
	inlist->filter = NULL;
	inlist->map = NULL;
	inlist->zip = NULL;
	inlist->take= NULL;
	free(inlist);
}

static ANY * __get_nth(FLIST *inlist, unsigned int ndx) {
	int i = 0;
	FLIST_NODE *curr = inlist->__s__head;
	if (ndx >= inlist->size)	
		return NULL;
	while(i < ndx){
		curr = curr->next;	
		i++;
	}
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data->copy(curr->wrapped_data);
}

static ANY * __head(FLIST *inlist) {
	FLIST_NODE *curr = inlist->__s__head;
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data->copy(curr->wrapped_data);
} 

static ANY * __last(FLIST *inlist) {
	FLIST_NODE *curr = inlist->__s__last;
	if(!curr)
		return NULL;
	else
		return curr->wrapped_data->copy(curr->wrapped_data);
}

static ANY * __i_fold_left(FLIST_NODE *node, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	ANY *res;
	if (node){
		res = fn(acc, node->wrapped_data);	
		return __i_fold_left(node->next, res, fn);
	}
	else {
		return acc;
	}
}

static ANY * __fold_left(FLIST *inlist, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (inlist->size == 0){
		return acc;
	}
	else {
		return __i_fold_left(inlist->__s__head, acc, fn);
	}
	
}

static ANY * __i_fold_right(FLIST_NODE *node, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (node){
		return fn(__i_fold_right(node->next, acc, fn),node->wrapped_data);	
	}
	else {
		return acc;
	}
}

static ANY * __fold_right(FLIST *inlist, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (inlist->size == 0){
		return acc;
	}
	else {
		return __i_fold_right(inlist->__s__head, acc, fn);
		
	}
}

static void __flip(FLIST *inlist) {
	FLIST_NODE *curr = inlist->__s__head;
	FLIST_NODE *hd = inlist->__s__head;
	FLIST_NODE *tl = inlist->__s__last;
	FLIST_NODE *next;
	FLIST_NODE *prev;
	while(curr != NULL){
		next = curr->next;
		prev = curr->prev;
		curr->prev = next;
		curr->next = prev; 
		curr = next;
	}
	inlist->__s__last = hd;
	inlist->__s__head = tl; 
}

static FLIST *__reverse(FLIST *inlist){
	FLIST *cp = inlist->copy(inlist);
	FLIST *ret = (FLIST *)__fold_left(cp, (ANY *)new_list(), (ANY * (*) (ANY *, ANY *))__prepend_list);
	__list_release_all_wrapped(cp);
	cp->delete(cp);
	return ret;
	
}

typedef struct map_struct{
	struct map_struct *this;
	void (*delete) (struct map_struct*);
	struct map_struct* (*copy) (struct map_struct *);
	FLIST	*acc;
	ANY * (*fn)(ANY *);
}MAP_STRUCT;

/**
 * Higher order function of map
 */
static MAP_STRUCT* __map_helper(MAP_STRUCT *instruct, ANY *in){
	ANY * out = instruct->fn(in); 
	instruct->acc->append(instruct->acc, out);
	return instruct;	
}

typedef struct filter_struct{
	struct filter_struct *this;
	void (*delete) (struct map_struct*);
	struct map_struct* (*copy) (struct map_struct *);
	FLIST	*acc;
	int  (*fn)(ANY *);
}FILTER_STRUCT;

/**
 * Higher order function of filter 
 */
static FILTER_STRUCT* __filter_helper(FILTER_STRUCT *instruct, ANY *in){
	int truth = instruct->fn(in); 
	if (truth)
		instruct->acc->append(instruct->acc, in->copy(in));
	return instruct;	
} 

/**
 * NAME			: __map
 * DESCRIPTION	: change a list of type x to list of type y
 * INPUT
 *		inlist	: list to tranform
 *		fn		: function with parameter of inlist member type
 *				  and return of desired output list member type
 *				  example list of type integer to list of type string
 * RETURNS		: new tranformed list. inlist is not mutated.
 */

static FLIST *__map(FLIST *inlist, ANY * (*fn)(ANY *)){	
	FLIST *nl = new_list();
	MAP_STRUCT ms;
	ms.acc = nl;
	ms.fn = fn;
	ms.this = &ms;
	ms.copy = NULL;
	ms.delete = NULL; 
	inlist->fold_left(inlist, (ANY *) &ms, (ANY *(*)(ANY*, ANY*))__map_helper);

	return nl;
}

/**
 * NAME			: __filter
 * DESCRIPTION	: create a new list that match fn condition
 * INPUT
 *		inlist	: list to tranform
 *		fn		: function which accept inlist member type
 *				  and return boolean *				  
 * RETURNS		: new tranformed list. inlist is not mutated.
 */
static FLIST * __filter(FLIST *inlist, int (*fn)(ANY *)){
	FLIST *nl = new_list();
	FILTER_STRUCT fs;
	fs.acc = nl;
	fs.fn = fn;
	fs.this = &fs;
	fs.copy = NULL;
	fs.delete = NULL; 
	inlist->fold_left(inlist, (ANY *) &fs, (ANY *(*)(ANY*, ANY*))__filter_helper);
	return nl;
}

typedef struct collect_struct{
	struct collect_struct *this;
	void (*delete) (struct collect_struct*);
	struct collect_struct* (*copy) (struct collect_struct *);
	FLIST	*acc;
	OPTION *(*fn)(ANY *);
}COLLECT_STRUCT; 

/**
 * Higher order function of collect 
 */
static COLLECT_STRUCT* __collect_helper(COLLECT_STRUCT *instruct, ANY *in){
	OPTION *opt = instruct->fn(in); 
	ANY * out; 
	if (opt->is_some(opt)) { 
		out = ((SOME *)opt)->get((SOME *)opt);
		out = out->copy(out);
		opt->delete(opt);	
		instruct->acc->append(instruct->acc, out);
	}
	return instruct;
} 

static FLIST *__collect(FLIST *inlist, OPTION* (*fn)(ANY *)){	
	FLIST *nl = new_list();
	COLLECT_STRUCT ms;
	ms.acc = nl;
	ms.fn = fn;
	ms.this = &ms;
	ms.copy = NULL;
	ms.delete = NULL; 
	inlist->fold_left(inlist, (ANY *) &ms, (ANY *(*)(ANY*, ANY*))__collect_helper);

	return nl;
}

static FLIST * __zip_helper(FLIST *outlist, FLIST_NODE *n1, FLIST_NODE *n2) {
	TUPLE_2 *t2;
	if (n1 && n2) {
		t2 = new_tuple2(n1->wrapped_data, n2->wrapped_data);	
		outlist->append(outlist, (ANY *)t2);
		return __zip_helper(outlist, n1->next, n2->next);
	}
	else {
		return outlist;
	}
}

static FLIST *__zip(FLIST *e0l, FLIST *e1l) {
	FLIST *nl = new_list();
	if (nl) {
		return __zip_helper(nl, e0l->__s__head, e1l->__s__head);
	}
	else
		return nl;
}


static FLIST *copy (FLIST *in) {
	FLIST *out = new_list();
	ANY *item;
	int i;
	if (out) {
		for (i = 0; i < in->size; i ++){
			item = in->get(in, i);
			out->append(out, item);
		}
		out->prepend = in->prepend;
		out->append = in->append;
		out->init = in->init;
		out->tail = in->tail;
		out->get = in->get;
		out->head = in->head;
		out->last= in->last;
		out->fold_left = in->fold_left;
		out->fold_right = in->fold_right;
		out->copy = in->copy;
		out->flip= in->flip;
		out->reverse= in->reverse;
		out->map = in->map;
		out->filter = in->filter;
		out->collect = in->collect;
		out->zip = in->zip;
	}
	return out;
}

static FLIST *__take_n_helper(FLIST_NODE *in, int n, FLIST *out) {
	if (n > 0 && in) {
		out = out->append(out, in->wrapped_data->copy(in->wrapped_data));
		return __take_n_helper(in->next, n - 1, out);
	}
	return out;

}

static FLIST *__take_n(FLIST *in, int n) {
	FLIST *out = NULL;
	if (in->size > 0 && (out = new_list())){
		return __take_n_helper(in->__s__head, n < in->size ? n : in->size, out);
	}
	else {
		return out;
	}
}

FLIST *new_list() {
	FLIST *ret = (FLIST *) calloc(1, sizeof(FLIST));
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
	ret->fold_left = __fold_left;
	ret->fold_right = __fold_right;
	ret->copy = copy;
	ret->flip= __flip;
	ret->reverse= __reverse;
	ret->map = __map;
	ret->filter = __filter;
	ret->collect = __collect;
	ret->zip= __zip;
	ret->take= __take_n;
	return ret;
}

FLIST *cons(ANY *hd, FLIST *tl){
	if (tl)
		tl->prepend(tl, hd);
	
	return tl;
}

CONS *uncons(FLIST *in){
	ANY *hd;
	CONS *nc = (CONS *) calloc(1, sizeof(CONS));
	if (!nc)
			return NULL;

	if (in && in->size && in->__s__head){
			hd = __release_wrapped_and_delete(in, in->__s__head);
			nc->hd = hd;
			nc->tl = in;
			return nc; 
	}
	else
		return NULL;
}

void free_cons(CONS *cons) {
	memset(cons, 0, sizeof(CONS));
	free(cons);
}

FLIST *list_create(int num_items, ...) {
	int i;
	va_list ap;
	FLIST *list = new_list();
	ANY *cur;
	if (list){
		va_start(ap, num_items);
		for(i = 0; i < num_items; i++){
			cur = va_arg(ap, ANY *);
			list->append(list, cur);
		}
		va_end(ap);
	} 
	return list;
}

#ifdef _FLIST_UNIT_TEST_
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include "integer.h"
#include "charstr.h"
void create_list(){
	printf("-create list\n");
	FLIST *t = new_list();
	assert(t != NULL);
	t->delete(t);
}

void prepend_list(){
	printf("-prepend list\n");
	FLIST *t = new_list();
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
	FLIST *t = new_list();
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
	FLIST *t = new_list();
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
	FLIST *t = new_list();
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
void list_get(){
	printf("-list_get\n");
	FLIST *t = new_list();
	FLIST_NODE *t2;
	FLIST_NODE *t3;
	FLIST_NODE *t4;
	CHARSTR *c1;
	assert(t != NULL);
	t->append(t, (ANY *)new_charstr("hello"));
	t->append(t, (ANY *)new_charstr("world"));
	t2 = t->__s__last;
	t->append(t, (ANY *)new_charstr("of"));
	t3 = t->__s__last;
	t->append(t, (ANY *)new_charstr("amazing"));
	t4 = t->__s__last;
	assert(!strcmp((c1 = (CHARSTR *)t->get(t, 3))->data, ((CHARSTR *)t4->wrapped_data)->data)); 
	c1->delete(c1);
	t->tail(t);
	assert(!strcmp((c1 = (CHARSTR *)t->get(t, 2))->data, ((CHARSTR *)t4->wrapped_data)->data));
	c1->delete(c1);
	assert(!strcmp((c1 = (CHARSTR *)t->get(t, 1))->data, ((CHARSTR *)t3->wrapped_data)->data));
	c1->delete(c1);
	assert(!strcmp((c1 = (CHARSTR *)t->get(t, 0))->data, ((CHARSTR *)t2->wrapped_data)->data));
	c1->delete(c1);
	t->init(t);
	t->delete(t);
}
void head_last(){
	printf("-head_last\n");
	FLIST *t = new_list();
	FLIST_NODE *t1;
	FLIST_NODE *t2;
	FLIST_NODE *t3;
	FLIST_NODE *t4;
	CHARSTR *c1;
	assert(t != NULL);
	t->append(t, (ANY *)new_charstr("hello"));
	t1 = t->__s__last;
	t->append(t, (ANY *)new_charstr("world"));
	t2 = t->__s__last;
	t->append(t, (ANY *)new_charstr("of"));
	t3 = t->__s__last;
	t->append(t, (ANY *)new_charstr("wonder"));
	t4 = t->__s__last;
	assert(!strcmp((c1 = (CHARSTR *)t->last(t))->data, ((CHARSTR *)t4->wrapped_data)->data)); 
	c1->delete(c1);
	assert(!strcmp((c1 = (CHARSTR *)t->head(t))->data, ((CHARSTR *)t1->wrapped_data)->data)); 
	c1->delete(c1);
	t->tail(t);
	assert(!strcmp((c1 = (CHARSTR *)t->head(t))->data, ((CHARSTR *)t2->wrapped_data)->data)); 
	c1->delete(c1);
	t->init(t);
	assert(!strcmp((c1 = (CHARSTR *)t->last(t))->data, ((CHARSTR *)t3->wrapped_data)->data)); 
	c1->delete(c1);
	t->delete(t);
}

Integer *add_str_length(Integer *in, CHARSTR *sin) {
	in->value += sin->len;
	return in;
}	

void fold_left() {
	printf("-fold_left\n");
	FLIST *l = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	Integer *res = (Integer *) l->fold_left(l, (ANY *)new_integer(0), (ANY * (*) (ANY *, ANY *)) add_str_length);
	assert(res->value == 21);
	l->delete(l); 
	res->delete(res);

}

void fold_right() {
	printf("-fold_right\n");
	FLIST *l = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	Integer *res = (Integer *) l->fold_right(l, (ANY *)new_integer(0), (ANY * (*) (ANY *, ANY *)) add_str_length);
	assert(res->value == 21);
	res->delete(res);
	l->delete(l); 
}

void copy_test() {
	printf("-copy_test\n");
	FLIST *inlist = list_create(11, (ANY *)new_charstr("Hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"), 
		(ANY *)new_charstr("!"), (ANY *)new_charstr("never"), (ANY *)new_charstr("forget"),
		(ANY *)new_charstr("your"), (ANY *)new_charstr("beginner's"), (ANY *)new_charstr("spirit"));
	FLIST *rev = inlist->copy(inlist);
	assert(inlist->size == rev->size);
	inlist->delete(inlist);
	rev->delete(rev);
} 

void reverse() {
	FLIST *nl;
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	CHARSTR *st;
	printf("-reverse\n"); 
	nl = inlist->reverse(inlist);
	st = (CHARSTR *)nl->head(nl);
	assert(!strcmp(st->data, "soul")); 
	st->delete(st);
	st = (CHARSTR *) inlist->get(inlist, 2);
	assert(!strcmp(st->data, "of")); 
	st->delete(st);
	nl->delete(nl);
	inlist->delete(inlist);
} 

void flip() {
	printf("-flip\n"); 
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	inlist->flip(inlist);
	CHARSTR *st = (CHARSTR *)inlist->head(inlist);
	assert(!strcmp(st->data, "soul"));
	st->delete(st);
	st = (CHARSTR *) inlist->get(inlist, 2);
	assert(!strcmp(st->data, "of")); 
	st->delete(st);
	inlist->delete(inlist);
	
}

Integer *char_len(CHARSTR *in){
	return new_integer(in->len);
}

int filter_fun(CHARSTR *in) {
	return (in->len > 4);
}

void map() {
	printf("-map\n"); 
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	FLIST *outlist = inlist->map(inlist, (ANY *(*)(ANY *)) char_len);

	Integer *res = (Integer *)outlist->get(outlist, 3);
	assert(res->value == 5);
	res->delete(res);
	res = (Integer *)outlist->get(outlist, 2);
	assert(res->value == 2);
	res->delete(res);
	
	outlist->delete(outlist);
	inlist->delete(inlist);	
}

void filter() {
	printf("-filter\n"); 
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	FLIST *outlist = inlist->filter(inlist, (int (*)(ANY *)) filter_fun);
	assert(outlist->size == 3);

	CHARSTR *res = (CHARSTR *)outlist->get(outlist, 2);
	assert(!strcmp(res->data, "brave"));
	res->delete(res);

	outlist->delete(outlist);
	inlist->delete(inlist);	
}

OPTION *collect_fun(CHARSTR *in) {
	if (in->data[0] == 'h' || in->data[0] == 's'){
		return (OPTION *)some_object((ANY *)in);
	}
	else
		return (OPTION *)none_object();
}

void collect_test() {
	printf("-collect_test\n");
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	FLIST *outlist = inlist->collect(inlist, (OPTION *(*)(ANY *)) collect_fun);
	CHARSTR *out = (CHARSTR *)outlist->head(outlist);
	assert(!strcmp(out->data, "hello"));
	out->delete(out);
	assert(outlist->size == 2); 
	outlist->delete(outlist);
	inlist->delete(inlist); 
}

void zip_test() {
	printf("-zip_test\n");
	CHARSTR *str0;
	CHARSTR *str1;
	TUPLE_2 *t1, *t2;
	FLIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
	FLIST *inlist2 = list_create(4, (ANY *)new_charstr("hallo"), (ANY *)new_charstr("dunia"), 
		(ANY *)new_charstr("milik"), (ANY *)new_charstr("pemberani"));
	FLIST *outlist = inlist->zip(inlist, inlist2);
	assert(outlist->size == 4); 
	str0 = (CHARSTR *)(t1 = (TUPLE_2 *)outlist->get(outlist, 0))->e0;
	str1 = (CHARSTR *)(t2 = (TUPLE_2 *)outlist->get(outlist, 0))->e1;
	assert(!strcmp(str0->data, "hello"));
	assert(!strcmp(str1->data, "hallo"));
	t1->delete(t1);
	t2->delete(t2);
	str0 = (CHARSTR *)(t1 = (TUPLE_2 *)outlist->get(outlist, 3))->e0;
	str1 = (CHARSTR *)(t2 = (TUPLE_2 *)outlist->get(outlist, 3))->e1;
	assert(!strcmp(str0->data, "brave"));
	assert(!strcmp(str1->data, "pemberani"));
	t1->delete(t1);
	t2->delete(t2);
	outlist->delete(outlist);
	inlist->delete(inlist); 
	inlist2->delete(inlist2); 
}

void take_test() {
	printf("-take_test\n");
	CHARSTR * c1;
	TUPLE_2 * t1;
	FLIST * outlist;
	FLIST * zipped;

	FLIST *inlist = list_create(11, (ANY *)new_charstr("Hello"), (ANY *)new_charstr("world"), 
		(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"), 
		(ANY *)new_charstr("!"), (ANY *)new_charstr("never"), (ANY *)new_charstr("forget"),
		(ANY *)new_charstr("your"), (ANY *)new_charstr("beginner's"), (ANY *)new_charstr("spirit"));

	FLIST *inlist2 = list_create(10, (ANY *)new_charstr("hallo"), (ANY *)new_charstr("dunia"), 
		(ANY *)new_charstr("milik"), (ANY *)new_charstr("pemberani"), (ANY *)new_charstr("jangan"),
		(ANY *)new_charstr("pernah"), (ANY *)new_charstr("lupakan"), (ANY *)new_charstr("semangatmu"),
		(ANY *)new_charstr("yang"), (ANY *)new_charstr("permulaan"));

	outlist = inlist->take(inlist, 6); 
	assert(outlist->size == 6);
	c1 = (CHARSTR *)outlist->last(outlist);
	assert(!strcmp(c1->data, "!"));
	c1->delete(c1);
	c1 = (CHARSTR *)outlist->get(outlist, 2);
	assert(!strcmp(c1->data, "of"));
	c1->delete(c1);
	outlist->delete(outlist);

	zipped = inlist->zip(inlist, inlist2);
	outlist = zipped->take(zipped, 17);
	t1 = (TUPLE_2*) outlist->get(outlist, 2);
	assert(!strcmp(((CHARSTR *)t1->e1)->data, "milik"));
	t1->delete(t1);
	outlist->delete(outlist);
	zipped->delete(zipped);
	inlist2->delete(inlist2);
	inlist->delete(inlist);
	
}

int unit_test() {
	create_list();
	prepend_list();
	append_list();
	init_list();
	tail_list();
	list_get();
	head_last();
	fold_left();
	fold_right();
	copy_test();
	reverse();
	flip();
	map();
	filter();
	collect_test();
	zip_test();
	take_test();
	return 0;
}

void mem_test() {
  while(1){
    unit_test();
  }
}
#endif
