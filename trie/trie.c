#include "trie.h"
#include <stdlib.h>
#include <memory.h>


void __trieitem_ctor (PTRIE_ITEM  new_item, PTRIE_ITEM parent, char name)
{
	new_item->key = name;
	new_item->head = new_item->tail = new_item->curr = new_item->next = 0;
	new_item->parent = parent;
}

PTRIE_ITEM newtrieitem(char name, PTRIE_ITEM parent){
	PTRIE_ITEM ret = (PTRIE_ITEM) calloc(1, sizeof (TRIE_ITEM));	
	if (ret){
		__trieitem_ctor(ret, parent, name);	
	}
	return ret;
}

static int trieitem_add (struct trie_item *root, struct trie_item *addedchild)
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
	return 0;
}

/**
 * NAME          : insert_child_int
 * DESCRIPTION   : find a place for a new key in list of node directly below 
 *                 parent node, if key exists return pointer to this child node
 *                 otherwise, create a new item, set as new child  and return the new child
 * INPUT
 *      parent   : parent of this key
 *      key      : key to insert
 *      cmp      : function which compare two integer key, returns 
 *				   0 if  param1 (key) has alredy occur in param2, 
 *				   1 if key should occur after param2
 *                -1 if key should occur before param2
 * RETURNS
 *		0        : failed
 *		other    : pointer to the newly added item or existing item
 *                
 */
PTRIE_ITEM insert_child(PTRIE_ITEM parent, char key, int (*cmp)(char, PTRIE_ITEM)) {
		PTRIE_ITEM prev = NULL, curr = NULL; 
		PTRIE_ITEM ntr;
		int occ = 1;//assume this key is the first key
		curr = parent->head;

		while(curr &&
			(occ = cmp(key, curr))> 0){
			prev = curr;
			curr = curr->next;
		}

		if (occ == 0) {
			return curr;
		} 
		
		ntr = newtrieitem(key, parent);

		if(!ntr)
			return NULL; 
		else if (occ > 0) {
			trieitem_add(parent, ntr);
			return ntr;
		}
		else {
			ntr->next = curr;
			if (prev){
				prev->next = ntr;
			}
			else{
				parent->head = ntr;
			}
			return ntr;
		}
}

PTRIE_ITEM find_child(PTRIE_ITEM parent, int key, int (*cmp)(char, PTRIE_ITEM)){
	PTRIE_ITEM curr = NULL; 
	int occ = -1;//assume this key is the first key
	

	if ((curr = parent->head) == NULL){//we are exhausted by key is not yet found
		return NULL;
	}
	
	while(curr &&
		curr->key != 0
		&&
		(occ = cmp(key, curr))> 0){
		curr = curr->next;
	}
	if (!curr || occ == 0)
		return curr;
	else if (curr->key == 0)
		return curr;
	else
		return NULL; 
}

int compare_char(char key, PTRIE_ITEM item) {
  char b = item->key;
	if (b == key)
		return 0;
	else if (key < b)
		return -1;
	else
		return 1;
}

PTRIE_ITEM insert_key(PTRIE_ITEM parent, char *inchar, int idx) {
	PTRIE_ITEM ret;
	if((ret = insert_child(parent, inchar[idx], compare_char)) && inchar[idx]!=0){
		return insert_key(ret, inchar, ++idx);
	}
	else{
		return ret;
	}
}

PTRIE_ITEM find_key(PTRIE_ITEM parent, char *inchar, int idx) {
	PTRIE_ITEM ret;
	if((ret = find_child(parent, inchar[idx], compare_char)) && ret->key){
		return find_key(ret, inchar, ++idx);
	}
	else{
		return ret;
	}
}

PTRIE_ITEM find_child_key(PTRIE_ITEM parent, char *inchar) {
	PTRIE_ITEM ret = find_key(parent, inchar, 0);
	
	while(ret && ret->parent != parent){
		ret = ret->parent;
	}
	return ret;
}

int delete_trie(PTRIE_ITEM parent) {
	PTRIE_ITEM curr = parent->head, to_del;
	while(curr) {
		to_del = curr;
		parent->head = curr->next;
		curr = curr->next;
		delete_trie(to_del);
	}
	memset (parent, 0, sizeof(TRIE_ITEM));
	free(parent);
	return 0;
}

#ifdef TRIE_TEST
/**
 * gcc -Wall -ggdb3 -I . -D TRIE_TEST trie.c -o trie_test
 */
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
void print_trie_test_res(PTRIE_ITEM ret, PTRIE_ITEM root){
	PTRIE_ITEM res[100] = {0};
	int nums = 0;

	if (ret == NULL){
		fprintf(stderr, "[EMPTY]\n");
		return;
	}
	while(ret && ret != root){
		res[nums++] = ret;
		ret = ret->parent;
	}
	
	while(--nums >= 0){
		fprintf(stdout, "%c", res[nums]->key);
	}
	fprintf (stdout, "\n");
}
void find_test(PTRIE_ITEM root, char *tofind) {
  fprintf (stderr, "finding %s => ", tofind);
  
	PTRIE_ITEM ret = find_key(root,  tofind, 0);
	print_trie_test_res(ret, root);
}

int test_test();
void usage(char *app) {
	fprintf(stdout, "%s [(a)ll case| -(h)elp/list case number -(m)emleak tet]\n", app); 
}
int main (int argc, char **argv) {
	static struct option long_options[]	 = {
		{"all", no_argument, 0, 'a'},
		{"help", no_argument, 0, 'h'},
		{"memleak", no_argument, 0, 'm'},
		{0, 0, 0, 0}
	};
	
	int option_index = 0;
	int opt;
	int optlong;
	while ((opt = getopt_long(argc, argv, "ahm", long_options,&option_index))!= -1) {
		switch (opt) {
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			case 'a':
			case 'm':
				optlong = opt;
				break;
			
		}
		
	}

	if (optlong == 'a'){
		return test_test();
	}
	else if (optlong == 'm'){
		do{
			test_test();
			sleep(1);
		}while (1);
		return 1;
	}

	return 0;
}

int test_test() { 
	PTRIE_ITEM root = newtrieitem((char) 255, NULL); 
	char k0[] = "6681132122";
	char k1[] = "66811321235";
	char k2[] = "21235";
	char k3[] = "11235";
	char k4[] = "11453435"; 
	insert_key(root, k0, 0);
	insert_key(root, k1, 0);
	insert_key(root, k2, 0);
	insert_key(root, k3, 0);
	insert_key(root, k4, 0); 
	insert_key(root, "1013272", 0);
	insert_key(root, "011", 0);
	insert_key(root, "0342343409", 0);
	insert_key(root, "01342343409", 0);
	insert_key(root, "01242343409", 0);
	insert_key(root, "02242343409", 0);
	insert_key(root, "02242343509", 0);
	insert_key(root, "0127343509", 0);
	insert_key(root, "7127343509", 0);
	insert_key(root, "75551509", 0);
	insert_key(root, "75552", 0);
	insert_key(root, "755522345569", 0);
	insert_key(root, "754432343333", 0);
	insert_key(root, "784443889348", 0);
	insert_key(root, "88444", 0);
	insert_key(root, "88555", 0);
	insert_key(root, "96555", 0);
	insert_key(root, "77555", 0);
	insert_key(root, "377555", 0);
	insert_key(root, "7439347149414", 0);
	insert_key(root, "74847294", 0);
	insert_key(root, "27982048274", 0);
	insert_key(root, "642403947", 0);
	insert_key(root, "3792472928", 0);
	insert_key(root, "8374747294", 0);
	insert_key(root, "73948274", 0);
	insert_key(root, "343549573", 0);
	insert_key(root, "43485727", 0);
	insert_key(root, "42423947", 0);
	insert_key(root, "382", 0);


	find_test(root, k1);
	find_test(root, "111");
	find_test(root, "0111");
	find_test(root, "11");
	find_test(root, "71");
	find_test(root, "766346");
	find_test(root, "22");
	find_test(root, "95358");
	find_test(root, "83957829");
	find_test(root, "4248573");
	find_test(root, "37755");
	find_test(root, "672593846");
	find_test(root, "583948503");
	find_test(root, "42423");
	find_test(root, "7592241");
	find_test(root, "6424039");
	find_test(root, "42983");
	find_test(root, "2123542983");
	find_test(root, "3833542983");
	find_test(root, "3832542983");
	find_test(root, "38223822");

	delete_trie(root);
	return 1;	
}
#endif//TRIE_TEST

