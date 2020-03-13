#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include "radixtree.h"
#include <stdio.h>

void __radixtreeitem_ctor(PRADIXTREE_ITEM new_item, PRADIXTREE_ITEM parent, char *name, int name_len) {
	if (name_len > 1){
		strncpy(new_item->name, name, name_len);
		new_item->name_len = name_len;
	}
	else if (name_len){
		new_item->name[0] = name[0];
		new_item->name_len = 1;
	}

	new_item->head = new_item->tail = new_item->next = 0;
	new_item->parent = parent;
}

PRADIXTREE_ITEM new_radixtreeitem(char *name, int name_len, PRADIXTREE_ITEM parent) {
	PRADIXTREE_ITEM ret = (PRADIXTREE_ITEM) calloc(1, sizeof (RADIXTREE_ITEM));	
	if (ret){
		__radixtreeitem_ctor(ret, parent, name, name_len);	
	}
	return ret; 
}

PRADIXTREE_ITEM radixtreeitem_add (struct radixtree_item *parent, struct radixtree_item *addedchild)
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
	addedchild->parent = parent;
	return addedchild;
}

/**
 * NAME						: radixtreeitem_insert_head
 * DESCRIPTION		: insert and item to head (first child) of a parent tree
 */
PRADIXTREE_ITEM radixtreeitem_insert_head(struct radixtree_item *parent, struct radixtree_item *addedchild)
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
	addedchild->parent = parent;
	return addedchild;
}

PRADIXTREE_ITEM insert_child(PRADIXTREE_ITEM parent, char *name) {
	int len = strlen(name);	

	PRADIXTREE_ITEM curr = NULL, prev = NULL;

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
			PRADIXTREE_ITEM fc = (PRADIXTREE_ITEM) calloc(1, sizeof (RADIXTREE_ITEM));
			memcpy(fc->name, &curr->name[idx], curr->name_len - idx);	
			fc->name_len =  curr->name_len - idx;
			fc->head = curr->head; fc->tail = curr->tail; fc->parent = curr;
			fc->is_word = curr->is_word;
			fc->next  = NULL;
			curr->is_word = 0;
			curr->head = curr->tail = fc;
			curr->name[idx] = 0;
			curr->name_len = idx;
			if (name[idx])
				return insert_child(curr, &name[idx]);
			else{
				curr->is_word = 1;
				return curr;
			}
		}
		else if (idx == curr->name_len){
			if (name[idx] == 0)
				curr->is_word = 1;
			if (name[idx])
				return insert_child(curr, &name[idx]);
			else{
				return curr;
			}
		}else {//new item longer, just add the rest
			return insert_child(curr, &name[idx]);
		}
	}
	else if(occ < 0){
		
		PRADIXTREE_ITEM p  = new_radixtreeitem(name, len, parent);
		if(prev) {
			p->next = prev->next;
			prev->next = p;
		}
		else {
			radixtreeitem_insert_head(parent, p);
		}
		p->is_word = 1;
		return p;
	}
	else {
		PRADIXTREE_ITEM p = radixtreeitem_add(parent, new_radixtreeitem(name, len, parent));
		p->is_word = 1;
		return p;
	}

	return NULL; 
}

PRADIXTREE_ITEM find_name(PRADIXTREE_ITEM parent, char *name){
	PRADIXTREE_ITEM curr = parent->head;	
	while(curr) {
		if(curr->name[0] == name[0]){
			if(!memcmp(curr->name, name, curr->name_len)){
				if (name[curr->name_len] == 0){
					if (curr->is_word)
						return curr;
					else
						return NULL;
				}
				else {
					return find_name(curr, &name[curr->name_len]);
				}
			}
			return NULL;
		}
		curr = curr->next;
	}
	return NULL;
}

PRADIXTREE_ITEM find_prefix(PRADIXTREE_ITEM parent, char *name){
	PRADIXTREE_ITEM curr = parent->head;
	while(curr) {
		if(curr->name[0] == name[0]){
			if(!memcmp(curr->name, name, curr->name_len)){
				if (curr->is_word)
					return curr;
				else
					return find_prefix(curr, &name[curr->name_len]);
			}
			return NULL;
		}
		curr = curr->next;
	}
	return NULL;
}

int delete_node(PRADIXTREE_ITEM node) {
	PRADIXTREE_ITEM curr, to_clean;
	curr = node->head;
	while(curr){
		to_clean = curr;
		curr = curr->next;
		delete_node(to_clean);
	}
	memset (node, 0, sizeof(RADIXTREE_ITEM));
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
static int __delete_key(PRADIXTREE_ITEM parent, char *name){
	PRADIXTREE_ITEM curr = parent->head;
	PRADIXTREE_ITEM prev = NULL;
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
							delete_node(curr);
							return 0;
						}
						else {//curr is head child of parent
							parent->head = curr->next;
							if(curr->next){
								delete_node(curr);
								return 0;
							}
							else {
								parent->tail = NULL;
								delete_node(curr);
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
									delete_node(curr);
									return 0;
								}
								else {
									//a head
									parent->head = curr->next;
									if(curr->next){
										delete_node(curr);
										return 0;
									}
									else {
										parent->tail = NULL;
										delete_node(curr);
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

void delete_key(PRADIXTREE_ITEM parent, char *name){
	
	__delete_key(parent, name);
}

void print_tree(PRADIXTREE_ITEM root, int tab_count){	
	for(int i = 0; i < tab_count; i++){
		fprintf(stdout, "\t");
	}
	fprintf(stdout, "name : %s, is_word : %d\n", root->name, root->is_word);
	PRADIXTREE_ITEM curr = root->head;
	while(curr){
		print_tree(curr, tab_count + 1);
		curr = curr->next;
	} 
}

#ifdef _RADIXTREE_INTERNAL_TEST_
/**
 * gcc -Wall -ggdb3 -I . radixtree.c  -o radixtree_test -D_RADIXTREE_INTERNAL_TEST_
 */
void print_reverse(PRADIXTREE_ITEM bottom) {
	fprintf(stdout, "%s", bottom->name);
	if (bottom->parent){
		fprintf(stdout, " <- ");
		print_reverse(bottom->parent);
	}
}

void find_child_and_print(PRADIXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding name %s  ", to_find);
	PRADIXTREE_ITEM curr = find_name(root, to_find);
	if(curr){
		print_reverse(curr);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
}

void find_prefix_and_print(PRADIXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding prefix %s  ", to_find);
	PRADIXTREE_ITEM curr = find_prefix(root, to_find);
	if(curr){
		print_reverse(curr);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
}

void delete_and_print(PRADIXTREE_ITEM root, char * to_find){
	fprintf(stdout, "delete key %s  \n", to_find);
	delete_key(root, to_find);
	print_tree(root, 0);
	
}

int main (int argc, char **argv) {
	PRADIXTREE_ITEM root;
looper:
	root = new_radixtreeitem(NULL, 0, NULL);
	insert_child(root, "hello");
	insert_child(root, "world");
	insert_child(root, "he");
	insert_child(root, "thank");
	insert_child(root, "busier");
	insert_child(root, "world");
	insert_child(root, "bustiere");
	insert_child(root, "stop complain");
	insert_child(root, "the");
	insert_child(root, "thereover");
	insert_child(root, "then");
	insert_child(root, "this");
	insert_child(root, "thus");
	insert_child(root, "criminal");
	insert_child(root, "crime");
	insert_child(root, "crimea");
	insert_child(root, "therefore");
	insert_child(root, "thereof");
	insert_child(root, "therein");
	insert_child(root, "stop asking");
	insert_child(root, "stop ball");
	insert_child(root, "therein");
	insert_child(root, "there");
	insert_child(root, "123423433");
	insert_child(root, "88888");
	insert_child(root, "77777");
	insert_child(root, "323256");
	insert_child(root, "124");
	insert_child(root, "324");
	
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
	delete_node(root);
	sleep(1);
	goto looper;

	return 0;
	
}
#endif

