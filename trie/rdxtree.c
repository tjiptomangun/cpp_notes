/**
 * author : henky <hanky.acbb@telogic.com.sg>
 */
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include "rdxtree.h"
#include <stdio.h>

PRDXTREE_CONTEXT_ITEM new_rdxtree_context_item(PRDXTREE_ITEM new_titem) {
	PRDXTREE_CONTEXT_ITEM new_litem =  (PRDXTREE_CONTEXT_ITEM) calloc(1, sizeof(RDXTREE_CONTEXT_ITEM));
	if(new_litem) {
		new_litem->item = new_titem;
	}
	return new_litem;
}

void rdxtree_find_context_delete(PRDXTREE_FIND_CONTEXT to_del) {
	PRDXTREE_CONTEXT_ITEM curr;
	PRDXTREE_CONTEXT_ITEM tmp;
	if (to_del) {
		curr = to_del->head;
		while(curr) {
			tmp = curr->next;
			curr->item = NULL;
			curr->next = NULL;
			free(curr);
			curr = tmp;
		}
		to_del->head = NULL;
		to_del->tail = NULL;
		to_del->reversed = 0;
		free(to_del);
	}
}

void rdxtree_find_context_reverse(PRDXTREE_FIND_CONTEXT ctx) {
	if (ctx->head && ctx->head->next) {
		PRDXTREE_CONTEXT_ITEM curr = ctx->head->next;
		PRDXTREE_CONTEXT_ITEM prev = ctx->head;
		PRDXTREE_CONTEXT_ITEM next = NULL;
		PRDXTREE_CONTEXT_ITEM head = ctx->head;
		PRDXTREE_CONTEXT_ITEM tail = ctx->tail;
		
		while (curr) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		ctx->head = tail;
		ctx->tail = head;
		ctx->tail->next = NULL;
	}
	return;
}

PRDXTREE_FIND_CONTEXT rdxtree_find_context_reverse_once(PRDXTREE_FIND_CONTEXT ctx) {
	if (!ctx->reversed) {
		rdxtree_find_context_reverse(ctx);
		ctx->reversed = 1;
	}
	return ctx;
}

PRDXTREE_FIND_CONTEXT rdxtree_find_context_add(PRDXTREE_FIND_CONTEXT ctx, PRDXTREE_CONTEXT_ITEM item) {
	if (ctx->head == NULL) {
		ctx->head = ctx->tail = item;
	}
	else {
		ctx->tail->next = item;
		ctx->tail = item;
	}
	return ctx;
}

PRDXTREE_FIND_CONTEXT rdxtree_find_context_ctor(PRDXTREE_FIND_CONTEXT ctx) {
	ctx->add_item = rdxtree_find_context_add;
	ctx->reverse_once = rdxtree_find_context_reverse_once;
	ctx->delete = rdxtree_find_context_delete;
	return ctx;
}

PRDXTREE_FIND_CONTEXT new_rdxtree_find_context() {
	PRDXTREE_FIND_CONTEXT ret = (PRDXTREE_FIND_CONTEXT )calloc(1, sizeof(RDXTREE_FIND_CONTEXT));
	if (ret) {
		return rdxtree_find_context_ctor(ret);
	}
	return ret;
}


void rdxtreeitem_ctor(PRDXTREE_ITEM new_item, char *name, int name_len) {
	if (name_len > 1){
		strncpy(new_item->name, name, name_len);
		new_item->name_len = name_len;
	}
	else if (name_len){
		new_item->name[0] = name[0];
		new_item->name_len = 1;
	}

	new_item->head = new_item->tail = new_item->next = 0;
}

PRDXTREE_ITEM new_rdxtreeitem(char *name, int name_len) {
	PRDXTREE_ITEM ret = (PRDXTREE_ITEM) calloc(1, sizeof (RDXTREE_ITEM));
	if (ret){
		rdxtreeitem_ctor(ret, name, name_len);	
	}
	return ret; 
}

PRDXTREE_ITEM rdxtreeitem_add (struct rdxtree_item *parent, struct rdxtree_item *addedchild)
{
	if (!parent->head)
	{
		parent->head = addedchild;
		parent->tail = addedchild;
	}
	else
	{
		parent->tail->next = addedchild;
		parent->tail = addedchild;
	}
	return addedchild;
}

/**
 * NAME						: rdxtreeitem_insert_head
 * DESCRIPTION		: insert and item to head (first child) of a parent tree
 */
PRDXTREE_ITEM rdxtreeitem_inserthead(struct rdxtree_item *parent, struct rdxtree_item *addedchild)
{
	if (!parent->head)
	{
		parent->head = addedchild;
		parent->tail = addedchild;	
	}
	else
	{
		addedchild->next = parent->head;
		parent->head = addedchild;
	}
	return addedchild;
}

PRDXTREE_ITEM rdxtreeitem_insertkey(PRDXTREE_ITEM parent, char *name) {
	int len = strlen(name);

	PRDXTREE_ITEM curr = NULL, prev = NULL;

	int occ; //1 occured after, 0 occured match, -1 occured before
	curr = parent->head;

	int idx = 0;
	occ = 1;
	prev = NULL;
	while(curr && (occ = name[0] - curr->name[0]) > 0){
		prev = curr;
		curr = curr->next;
	}

	
	if (occ == 0){
		while(curr->name[idx] && name[idx] && curr->name[idx] == name[idx])
			idx++;
		// now idx is length of equal char
		if (idx < curr->name_len){
			PRDXTREE_ITEM fc = (PRDXTREE_ITEM) calloc(1, sizeof (RDXTREE_ITEM));
			memcpy(fc->name, &curr->name[idx], curr->name_len - idx);
			fc->name_len =  curr->name_len - idx;
			fc->head = curr->head; fc->tail = curr->tail;
			fc->is_word = curr->is_word;
			PRDXTREE_ITEM fcch = fc->head ;
			while(fcch) {
				fcch = fcch->next;
			}
			fc->next  = NULL;
			curr->is_word = 0;
			curr->head = curr->tail = fc;
			curr->name[idx] = 0;
			curr->name_len = idx;
			if (name[idx])
				return rdxtreeitem_insertkey(curr, &name[idx]);
			else{
				curr->is_word = 1;
				return curr;
			}
		}
		else if (idx == curr->name_len){
			if (name[idx] == 0)
				curr->is_word = 1;
			if (name[idx])
				return rdxtreeitem_insertkey(curr, &name[idx]);
			else{
				return curr;
			}
		}else {//new item longer, just add the rest
			return rdxtreeitem_insertkey(curr, &name[idx]);
		}
	}
	else if(occ < 0){
		
		PRDXTREE_ITEM p  = new_rdxtreeitem(name, len);
		if (p) {
			if(prev) {
				p->next = prev->next;
				prev->next = p;
			}
			else {
				rdxtreeitem_inserthead(parent, p);
			}
			p->is_word = 1;
		}
		return p;
	}
	else {
		PRDXTREE_ITEM newi = new_rdxtreeitem(name, len);
		if (!newi) {
			return newi;
		}
		PRDXTREE_ITEM p = rdxtreeitem_add(parent, newi);
		p->is_word = 1;
		return p;
	}

	return NULL; 
}

PRDXTREE_ITEM rdxtreeitem_findkey(PRDXTREE_ITEM parent, char *name, PRDXTREE_FIND_CONTEXT ctx){
	PRDXTREE_ITEM curr = parent->head;	
	while(curr) {
		if(curr->name[0] == name[0]){
			if(!memcmp(curr->name, name, curr->name_len)){
				ctx->add_item(ctx, new_rdxtree_context_item(curr));
				if (name[curr->name_len] == 0){
					if (curr->is_word)
						return curr;
					else
						return NULL;
				}
				else {
					
					return rdxtreeitem_findkey(curr, &name[curr->name_len], ctx);
				}
			}
			return NULL;
		}
		curr = curr->next;
	}
	return NULL;
}

PRDXTREE_ITEM rdxtreeitem_findprefix(PRDXTREE_ITEM parent, char *name, PRDXTREE_FIND_CONTEXT ctx){
	PRDXTREE_ITEM curr = parent->head;
	while(curr) {
		if(curr->name[0] == name[0]){
			if(!memcmp(curr->name, name, curr->name_len)){
				ctx->add_item(ctx, new_rdxtree_context_item(curr));
				if (curr->is_word)
					return curr;
				else
					return rdxtreeitem_findprefix(curr, &name[curr->name_len], ctx);
			}
			return NULL;
		}
		curr = curr->next;
	}
	return NULL;
}

int rdxtreeitem_deletenode(PRDXTREE_ITEM node) {
	PRDXTREE_ITEM curr, to_clean;
	curr = node->head;
	while(curr){
		to_clean = curr;
		curr = curr->next;
		rdxtreeitem_deletenode(to_clean);
	}
	memset (node, 0, sizeof(RDXTREE_ITEM));
	free(node);
	return 1;	
}

/**
 * NAME						: __delete_key
 * DESCRIPTION		: delete a key from a parent - internal function.
 * 									recursive, if callee returns
 * 									non null caller need to check if returning
 * 									child 
 * INPUT
 * 				parent	: parent contains keys
 * 				name		: key to delete
 * RETURNS
 */
static int __delete_key(PRDXTREE_ITEM parent, char *name){
	PRDXTREE_ITEM curr = parent->head;
	PRDXTREE_ITEM prev = NULL;
	while(curr) {
		if(curr->name[0] == name[0]){
			if(!memcmp(curr->name, name, curr->name_len)){
				if (curr->is_word && name[curr->name_len] == 0){
					curr->is_word = 0;
					if(curr->head == NULL){
						//delete this, what if parent also just one child ?
						if(prev){
							if (parent->tail == curr){
								parent->tail = prev;
							}
							prev->next= curr->next;
							rdxtreeitem_deletenode(curr);
							return 0;
						}
						else {//curr is head child of parent
							parent->head = curr->next;
							if(curr->next){
								rdxtreeitem_deletenode(curr);
								return 0;
							}
							else {
								parent->tail = NULL;
								rdxtreeitem_deletenode(curr);
								return 1; //grand parent need to check if its head is null
							}
						}
					}
					else {//match but not an empty, just leave it
						return 0;
					}
				}
				else {//match but we are not there yet, dig down
					
					//recursive but non tail call because we need to the result.
					//if non null may be we should delete the child also
					if (__delete_key(curr, &name[curr->name_len])){
						if (curr->head == NULL){
							//match and need to delete
							if (curr->is_word){//is a word parent, so do not delete
								return 0;
							}
							else {//not a word and empty, can delete
								if (prev){
									prev->next = curr->next;
									if(parent->tail == curr){
										parent->tail = prev;
									}
									rdxtreeitem_deletenode(curr);
									return 0;
								}
								else {
									//a head
									parent->head = curr->next;
									if(curr->next){
										rdxtreeitem_deletenode(curr);
										return 0;
									}
									else {
										parent->tail = NULL;
										rdxtreeitem_deletenode(curr);
										return 1; //grand parent need to check if its head is null
									}
								}
							}
						}
						else {//not empty node, just leave it
							return 0;
						}
					}
					else {
						return 0;
					}
				}
			}
			else {
				return 0;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	return 0;
}

void rdxtreeitem_deletekey(PRDXTREE_ITEM parent, char *name){
	
	__delete_key(parent, name);
}

void print_tree(PRDXTREE_ITEM root, int tab_count){	
	for(int i = 0; i < tab_count; i++){
		fprintf(stdout, "\t");
	}
	fprintf(stdout, "name : %s, is_word : %d\n", root->name, root->is_word);
	PRDXTREE_ITEM curr = root->head;
	while(curr){
		print_tree(curr, tab_count + 1);
		curr = curr->next;
	} 
}



PRDXTREE_CONTEXT_ITEM rdxtree_stack_context_push(PRDXTREE_STACK_CONTEXT ctx, PRDXTREE_CONTEXT_ITEM item) { 
	if (ctx) {
		item->next = ctx->top;
		ctx->top = item;
		return item;
	}
	return NULL;
}

PRDXTREE_CONTEXT_ITEM rdxtree_stack_context_pop(PRDXTREE_STACK_CONTEXT ctx) { 
	if (ctx && ctx->top) {
		PRDXTREE_CONTEXT_ITEM item = ctx->top;
		ctx->top = ctx->top->next;
		return item;
	}
	return NULL;
}

void rdxtree_stack_context_delete(PRDXTREE_STACK_CONTEXT ctx) {
	if (ctx) {
			PRDXTREE_CONTEXT_ITEM curr = ctx->top;
			PRDXTREE_CONTEXT_ITEM tmp; 
			while(curr) {
				tmp = curr->next;
				curr->item = NULL;
				curr->next = NULL;
				free(curr);
				curr = tmp;
			}
			ctx->delete = NULL;
			ctx->push = NULL;
			ctx->pop = NULL;
			ctx->top = 0;
			free(ctx);
	}
}

/**
 * NAME					: strreverse
 * DESCRIPTION	: reverse a string without changing input value
 * INPUT	
 * 	in					: input string
 * 	out					: the result
 * 	out_len_max	: maximum length output can store
 * RETURNS
 * 	-1					: can not copy, outlen_max exceeds
 * OTHERS				: string copied
 */
int strreverse(char *in, char *out, int outlen_max) {
	int len = strlen(in);
	int i = 0;
	if (len > outlen_max) {
		return -1;
	}
	else {
		for(i = 0; i < len/2; i++) {
			out[i] = in[len - i - 1];
			out[len - i - 1] = in[i];
		}
		if (len & 0x01) {
			out[i] = in[i];
		}
		return len;
	}
}

static void reverse0 (char *inbuf, int len){
	int tmp;
	int i;

	if (len<=1)
		return;
	for (i = 0; i < len/2; i++){
			tmp = inbuf[i];
			inbuf[i] = inbuf[len - i - 1];
			inbuf[len - i - 1] = tmp;
	}
}

void __rdxtreeitem_getkeywords(PRDXTREE_ITEM item, PRDXTREE_STACK_CONTEXT ctx, char *buffout, int buffout_max, int *buffout_count, char *delim) {
	int tmp_buffout = *buffout_count;
	int copied = 0;
	int delim_len = strlen(delim);
	if (item->is_word) {
		if ((copied = strreverse(item->name,&buffout[tmp_buffout], buffout_max - (tmp_buffout + delim_len))) < 0) {
			memset(&buffout[*buffout_count], 0, tmp_buffout - *buffout_count);
			return;
		}
		else {
			tmp_buffout += copied;
		}
		PRDXTREE_CONTEXT_ITEM curr= ctx->top;
		while (curr) {
			if ((copied = strreverse(curr->item->name,&buffout[tmp_buffout], buffout_max - (tmp_buffout + delim_len))) < 0) {
				memset(&buffout[*buffout_count], 0, tmp_buffout - *buffout_count);
				return;
			}
			else {
				tmp_buffout += copied;
			}
			curr = curr->next;
		}
		reverse0(&buffout[*buffout_count], tmp_buffout - *buffout_count);
		strcpy(&buffout[tmp_buffout], delim);
		*buffout_count = tmp_buffout + delim_len;
	}
	PRDXTREE_CONTEXT_ITEM citem = new_rdxtree_context_item(item);
	ctx->push(ctx, new_rdxtree_context_item(item));
	PRDXTREE_ITEM curr = item->head;
	while(curr) {
		__rdxtreeitem_getkeywords(curr, ctx, buffout, buffout_max, buffout_count, delim);
		curr = curr->next;
	}
	citem = ctx->pop(ctx);
	free(citem);
	citem = NULL;
}

PRDXTREE_STACK_CONTEXT  rdxtree_stack_context_ctor(PRDXTREE_STACK_CONTEXT  curr) {
	if(curr) {
		curr->push = rdxtree_stack_context_push;
		curr->pop = rdxtree_stack_context_pop;
		curr->delete = rdxtree_stack_context_delete;
	}
	return curr;
	
}
PRDXTREE_STACK_CONTEXT new_rdxtree_stack_context(){
	PRDXTREE_STACK_CONTEXT ret = (PRDXTREE_STACK_CONTEXT) calloc(1, sizeof(RDXTREE_STACK_CONTEXT));
	if(ret) {
		return rdxtree_stack_context_ctor(ret);
	}
	return ret;
}

void rdxtreeitem_getkeywords(PRDXTREE_ITEM item, char *bufout, int bufout_max, int *bufout_count, char *delim) {
	PRDXTREE_STACK_CONTEXT ctx = new_rdxtree_stack_context();
	__rdxtreeitem_getkeywords(item, ctx, bufout, bufout_max, bufout_count, delim);
	
}

#ifdef _RDXTREE_INTERNAL_TEST_
/**
 * gcc -Wall -ggdb3 -I . rdxtree.c  -o rdxtree_test -D_RDXTREE_INTERNAL_TEST_
 */
void print_reverse(PRDXTREE_FIND_CONTEXT ctx) {
	if (ctx) {
		ctx->reverse_once(ctx);
	}
	PRDXTREE_CONTEXT_ITEM curr = ctx->head;
	while (curr) {
		fprintf(stdout, " %s ", curr->item->name);
		fprintf(stdout, " <- ");
		curr = curr->next;
	}
	
}

void find_child_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding name %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findkey(root, to_find, ctx);
	if(curr){
		print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
	ctx->delete(ctx);
	ctx = NULL;
}

void find_prefix_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding prefix %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findprefix(root, to_find, ctx);
	if(curr){
		print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	}
	ctx->delete(ctx);
	ctx = NULL;
}

void delete_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "delete key %s  \n", to_find);
	rdxtreeitem_deletekey(root, to_find);
	print_tree(root, 0);
	
}

int main (int argc, char **argv) {
	PRDXTREE_ITEM root;
	root = new_rdxtreeitem(NULL, 0);
	rdxtreeitem_insertkey(root, "hello");
	rdxtreeitem_insertkey(root, "world");
	rdxtreeitem_insertkey(root, "he");
	rdxtreeitem_insertkey(root, "thank");
	rdxtreeitem_insertkey(root, "busier");
	rdxtreeitem_insertkey(root, "world");
	rdxtreeitem_insertkey(root, "bustiere");
	rdxtreeitem_insertkey(root, "stop complain");
	rdxtreeitem_insertkey(root, "the");
	rdxtreeitem_insertkey(root, "thereover");
	rdxtreeitem_insertkey(root, "then");
	rdxtreeitem_insertkey(root, "this");
	rdxtreeitem_insertkey(root, "thus");
	rdxtreeitem_insertkey(root, "criminal");
	rdxtreeitem_insertkey(root, "crime");
	rdxtreeitem_insertkey(root, "crimea");
	rdxtreeitem_insertkey(root, "therefore");
	rdxtreeitem_insertkey(root, "thereof");
	rdxtreeitem_insertkey(root, "therein");
	rdxtreeitem_insertkey(root, "stop asking");
	rdxtreeitem_insertkey(root, "stop ball");
	rdxtreeitem_insertkey(root, "therein");
	rdxtreeitem_insertkey(root, "there");
	rdxtreeitem_insertkey(root, "123423433");
	rdxtreeitem_insertkey(root, "88888");
	rdxtreeitem_insertkey(root, "77777");
	rdxtreeitem_insertkey(root, "323256");
	rdxtreeitem_insertkey(root, "124");
	rdxtreeitem_insertkey(root, "324");
	
	fprintf(stdout, "----- * -----\n");
	
	find_child_and_print(root, "he");
	find_child_and_print(root, "thus");
	find_child_and_print(root, "crimean");
	find_child_and_print(root, "stop");
	find_child_and_print(root, "crimea");

	fprintf(stdout, "----- * -----\n");
	find_prefix_and_print(root, "crimson");
	find_prefix_and_print(root, "crimealand");
	find_prefix_and_print(root, "thereof");
	find_prefix_and_print(root, "theresa mae");
	find_prefix_and_print(root, "busier");
	find_prefix_and_print(root, "323");
	find_prefix_and_print(root, "12464");
	find_prefix_and_print(root, "323256323");
	find_prefix_and_print(root, "8888555");
	find_prefix_and_print(root, "7777755");
	
	fprintf(stdout, "----- * -----\n");
	print_tree(root, 0);
	
	delete_and_print(root, "crimea");
	delete_and_print(root, "stop");
	delete_and_print(root, "7777755");
	delete_and_print(root, "the");
	delete_and_print(root, "than");
	find_child_and_print(root, "the");
	char buff[1000] = {0};
	int count = 0;
	rdxtreeitem_getkeywords(root, buff, sizeof(buff), &count, ",");
	fprintf(stdout, "%s\n", buff);
	rdxtreeitem_deletenode(root);

	return 0;
	
}
#endif

