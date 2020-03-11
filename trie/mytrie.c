#include <stdlib.h>
#include <memory.h>
#include "mytrie.h"

void __mytrieitem_ctor(PMYTRIE_ITEM new_item, PMYTRIE_ITEM parent, char *name, int name_len) {
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

PMYTRIE_ITEM new_mytrieitem(char *name, int name_len, PMYTRIE_ITEM parent) {
	PMYTRIE_ITEM ret = (PMYTRIE_ITEM) calloc(1, sizeof (MYTRIE_ITEM));	
	if (ret){
		__mytrieitem_ctor(ret, parent, name, name_len);	
	}
	return ret; 
}


PMYTRIE_ITEM mytrieitem_add (struct mytrie_item *root, struct mytrie_item *addedchild)
{
	if (!root->head)
	{
		root->head = addedchild;
		root->tail = addedchild;	
	}
	else
	{
		root->tail->next = addedchild;
		root->tail = addedchild;
	}
	addedchild->parent = root;
	return addedchild;
}

PMYTRIE_ITEM mytrieitem_insert_head(struct mytrie_item *root, struct mytrie_item *addedchild)
{
	if (!root->head)
	{
		root->head = addedchild;
		root->tail = addedchild;	
	}
	else
	{
		addedchild->next = root->head;
		root->head = addedchild;
	}
	addedchild->parent = root;
	return addedchild;
}

PMYTRIE_ITEM insert_child(PMYTRIE_ITEM parent, char *name) {
	int len = strlen(name);	

	PMYTRIE_ITEM curr = NULL, prev = NULL;

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
			PMYTRIE_ITEM fc = (PMYTRIE_ITEM) calloc(1, sizeof (MYTRIE_ITEM));
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
		
		PMYTRIE_ITEM p  = new_mytrieitem(name, len, parent);
		if(prev) {
			p->next = prev->next;
			prev->next = p;
		}
		else {
			mytrieitem_insert_head(parent, p);
		}
		p->is_word = 1;
		return p;
	}
	else {
		PMYTRIE_ITEM p = mytrieitem_add(parent, new_mytrieitem(name, len, parent));
		p->is_word = 1;
		return p;
	}

	return NULL; 
}

PMYTRIE_ITEM find_child(PMYTRIE_ITEM parent, char *name){
	PMYTRIE_ITEM curr = parent->head;	
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
					return find_child(curr, &name[curr->name_len]);
				}
			}
			return NULL;
		}
		curr = curr->next;
	}
	return NULL;
}

#ifdef _MYTRIE_TEST_
/**
 * gcc -Wall -ggdb3 -I . mytrie.c  -o mytrie_test -D_MYTRIE_TEST_
 */
#include <stdio.h>
void print_reverse(PMYTRIE_ITEM bottom) {
	fprintf(stdout, "%s", bottom->name);
	if (bottom->parent){
		fprintf(stdout, " <- ");
		print_reverse(bottom->parent);
	}
}

void print_tree(PMYTRIE_ITEM root, int tab_count){
	for(int i = 0; i < tab_count; i++){
		fprintf(stdout, "\t");
	}
	fprintf(stdout, "name : %s, is_word : %d\n", root->name, root->is_word);
	PMYTRIE_ITEM curr = root->head;
	while(curr){
		print_tree(curr, tab_count + 1);
		curr = curr->next;
	}
	
}

void find_child_and_print(PMYTRIE_ITEM root, char * to_find){
	fprintf(stdout, "finding %s  ", to_find);
	PMYTRIE_ITEM curr = find_child(root, to_find);
	if(curr){
		print_reverse(curr);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	}
	
}
int main (int argc, char **argv) {
	PMYTRIE_ITEM root = new_mytrieitem(NULL, 0, NULL);
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

	
	find_child_and_print(root, "he");
	find_child_and_print(root, "thus");
	find_child_and_print(root, "crimean");
	find_child_and_print(root, "stop");
	find_child_and_print(root, "crimea");
	

	print_tree(root, 0);
	return 0;
	
}
#endif

