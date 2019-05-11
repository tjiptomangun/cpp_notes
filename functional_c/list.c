#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <list.h>
#include <any.h>
#include <memory.h>
#include <stdarg.h> 

/**
 * NAME			: __new_list_node
 * DESCRIPTION	: create a new list node
 * INPUT
 *		prev	: the predecessor of this new node
 *		next	: the successor of this new node
 *		wrapped	: the data wrapped by this node
 */
static LIST_NODE* __new_list_node(ANY* wrapped, LIST_NODE *prev, LIST_NODE *next) {
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
 * DESCRIPTION	: delete the wrapped data, detach from other member
 */
static void __delete_list_node(LIST *ls, LIST_NODE *node) {
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
static LIST_NODE* __detach_list_node(LIST_NODE *node) {
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
static ANY* __release_wrapped_data(LIST_NODE *node) {
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
static ANY* __release_wrapped_and_delete(LIST *ls, LIST_NODE *node){
	ANY *ret = __release_wrapped_data(node);
	__delete_list_node(ls, node);
	return ret;
}

/**
 * NAME			: __unwrap_list_node
 * DESCRIPTION	: return wrapped data
 */
/*
static ANY *__unwrap_list_node(LIST_NODE *node){
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
static LIST *__tail_list(LIST *inlist){
	LIST_NODE *temp;
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
	inlist->fold_left= NULL;
	inlist->fold_right= NULL;
	inlist->flip= NULL;
	inlist->reverse= NULL;
	inlist->map= NULL;
	inlist->delete= NULL;
	inlist->collect= NULL;
	inlist->filter= NULL;
	inlist->map= NULL;
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

static ANY * __i_fold_left(LIST_NODE *node, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	ANY *res;
	if (node){
		res = fn(acc, node->wrapped_data);	
		return __i_fold_left(node->next, res, fn);
	}
	else {
		return acc;
	}
}

static ANY * __fold_left(LIST *inlist, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (inlist->size == 0){
		return acc;
	}
	else {
		return __i_fold_left(inlist->__s__head, acc, fn);
	}
	
}

static ANY * __i_fold_right(LIST_NODE *node, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (node){
		return fn(__i_fold_right(node->next, acc, fn),node->wrapped_data);	
	}
	else {
		return acc;
	}
}

static ANY * __fold_right(LIST *inlist, ANY *acc, ANY *(*fn)(ANY*, ANY*)){
	if (inlist->size == 0){
		return acc;
	}
	else {
		return __i_fold_right(inlist->__s__head, acc, fn);
		
	}
}

static LIST *__flip(LIST *inlist) {
	LIST_NODE *curr = inlist->__s__head;
	LIST_NODE *hd = inlist->__s__head;
	LIST_NODE *tl = inlist->__s__last;
	LIST_NODE *next;
	LIST_NODE *prev;
	while(curr != NULL){
		next = curr->next;
		prev = curr->prev;
		curr->prev = next;
		curr->next = prev; 
		curr = next;
	}
	inlist->__s__last = hd;
	inlist->__s__head = tl;
	return inlist;
}

static LIST *__reverse(LIST *inlist){
	return (LIST *)__fold_left(inlist, (ANY *)new_list(), (ANY * (*) (ANY *, ANY *))__prepend_list);
}

typedef struct map_struct{
	struct map_struct *this;
	void (*delete) (struct map_struct*);
	struct map_struct* (*copy) (struct map_struct *);
	LIST	*acc;
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
	LIST	*acc;
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

static LIST *__map(LIST *inlist, ANY * (*fn)(ANY *)){	
	LIST *nl = new_list();
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
static LIST * __filter(LIST *inlist, int (*fn)(ANY *)){
	LIST *nl = new_list();
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
	LIST	*acc;
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

static LIST *__collect(LIST *inlist, OPTION* (*fn)(ANY *)){	
	LIST *nl = new_list();
	COLLECT_STRUCT ms;
	ms.acc = nl;
	ms.fn = fn;
	ms.this = &ms;
	ms.copy = NULL;
	ms.delete = NULL; 
	inlist->fold_left(inlist, (ANY *) &ms, (ANY *(*)(ANY*, ANY*))__collect_helper);

	return nl;
}


static LIST *copy (LIST *in) {
	LIST *out = new_list();
	ANY *item;
	ANY *cp;
	int i;
	if (out) {
		for (i = 0; i < in->size; i ++){
			item = in->get(in, i);
			cp = item->copy(item);
			out->append(out, cp);		
		}
	}
	return out;
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
	ret->fold_left = __fold_left;
	ret->fold_right = __fold_right;
	ret->copy = copy;
	ret->flip= __flip;
	ret->reverse= __reverse;
	ret->map = __map;
	ret->filter = __filter;
	ret->collect = __collect;
	return ret;
}

LIST *cons(ANY *hd, LIST *tl){
	if (tl)
		tl->prepend(tl, hd);
	
	return tl;
}

CONS *uncons(LIST *in){
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

LIST *list_create(int num_items, ...) {
	int i;
	va_list ap;
	LIST *list = new_list();
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

#ifdef _LIST_UNIT_TEST_
#include <integer.h>
#include <charstr.h>
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
	void list_get(){
		printf("-list_get\n");
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

	Integer *add_str_length(Integer *in, CHARSTR *sin) {
		in->value += sin->len;
		return in;
	}	

	void fold_left() {
		printf("-fold_left\n");
		LIST *l = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		Integer *res = (Integer *) l->fold_left(l, (ANY *)new_integer(0), (ANY * (*) (ANY *, ANY *)) add_str_length);
		assert(res->value == 21);
		l->delete(l); 
		res->delete(res);
	
	}

	void fold_right() {
		printf("-fold_right\n");
		LIST *l = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		Integer *res = (Integer *) l->fold_right(l, (ANY *)new_integer(0), (ANY * (*) (ANY *, ANY *)) add_str_length);
		assert(res->value == 21);
		res->delete(res);
		l->delete(l); 
	}

	void reverse() {
		LIST *nl;
		LIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		CHARSTR *st;
		printf("-reverse\n"); 
		nl = inlist->reverse(inlist);
		st = (CHARSTR *)nl->head(nl);
		assert(!strcmp(st->data, "soul")); 
		st = (CHARSTR *) inlist->get(inlist, 2);
		assert(!strcmp(st->data, "of")); 
		inlist->delete(inlist);
	} 

	void flip() {
		printf("-flip\n"); 
		LIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		inlist->flip(inlist);
		CHARSTR *st = (CHARSTR *)inlist->head(inlist);
		assert(!strcmp(st->data, "soul"));
		st = (CHARSTR *) inlist->get(inlist, 2);
		assert(!strcmp(st->data, "of")); 
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
		LIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		LIST *outlist = inlist->map(inlist, (ANY *(*)(ANY *)) char_len);

		Integer *res = (Integer *)outlist->get(outlist, 3);
		assert(res->value == 5);
		res = (Integer *)outlist->get(outlist, 2);
		assert(res->value == 2);
		
		outlist->delete(outlist);
		inlist->delete(inlist);	
	}

	void filter() {
		printf("-filter\n"); 
		LIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		LIST *outlist = inlist->filter(inlist, (int (*)(ANY *)) filter_fun);
		assert(outlist->size == 3);

		CHARSTR *res = (CHARSTR *)outlist->get(outlist, 2);
		assert(!strcmp(res->data, "brave"));

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
		LIST *inlist = list_create(5, (ANY *)new_charstr("hello"), (ANY *)new_charstr("world"), 
			(ANY *)new_charstr("of"), (ANY *)new_charstr("brave"), (ANY *)new_charstr("soul"));
		LIST *outlist = inlist->collect(inlist, (OPTION *(*)(ANY *)) collect_fun);
		CHARSTR *out = (CHARSTR *)outlist->head(outlist);
		assert(!strcmp(out->data, "hello"));
		assert(outlist->size == 2); 
		
	}

	int main (int argc, char **argv) {
		create_list();
		prepend_list();
		append_list();
		init_list();
		tail_list();
		list_get();
		head_last();
		fold_left();
		fold_right();
		reverse();
		flip();
		map();
		filter();
		collect_test();
	}
#endif
