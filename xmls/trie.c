#include "parserclass.h"
#include <stdlib.h>
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
PTREE_ITEM insert_child_char(PTREE_ITEM parent, int key, int (*cmp)(char, char)) {
	PTREE_ITEM prev = NULL, curr = NULL; 
	char str_item[20] = {0};
	PTREE_ITEM ntr;
	int occ = 1;//assume this key is the first key
	curr = parent->getfirstchild(parent);

	while(curr &&
		(occ = cmp(key, curr->list.l_item.class.name[0]))> 0){
		prev = curr;
		curr = parent->getnextchild(parent);
	}

	if (occ == 0) {
		return curr;
	}

	str_item[0] = (char) key;
	ntr = newtreeitem(parent, str_item);

	if(!ntr)
		return NULL; 
	else if (occ > 0) {
		parent->add(parent, ntr);
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

PTREE_ITEM find_child_char(PTREE_ITEM parent, int key, int (*cmp)(char, char)){
	PTREE_ITEM curr = NULL; 
	int occ = -1;//assume this key is the first key
	
	if (key == 0){
		return parent;
	}
	else if ((curr = parent->getfirstchild(parent)) == NULL){//we are exhausted by key is not yet found
		return NULL;
	}
	
	while(curr &&
		(occ = cmp(key, curr->list.l_item.class.name[0]))> 0){
		curr = parent->getnextchild(parent);
	}
	
	if (occ == 0){
		return curr;
	}
	else
		return NULL;
	
	
}

int compare_char(char key, char b) {
	if (b == key)
		return 0;
	else if (key < b)
		return -1;
	else
		return 1;
}
/**
 *
 */
PTREE_ITEM insert_key_char(PTREE_ITEM parent, char *inchar, int idx) {
	PTREE_ITEM ret;
	if((ret = insert_child_char(parent, inchar[idx], compare_char)) && inchar[idx]!=0){
		return insert_key_char(ret, inchar, ++idx);
	}
	else{
		return ret;
	}
}

PTREE_ITEM find_key_char(PTREE_ITEM parent, char *inchar, int idx) {
	PTREE_ITEM ret;
	if((ret = find_child_char(parent, inchar[idx], compare_char)) && inchar[idx]!=0){
		return find_key_char(ret, inchar, ++idx);
	}
	else{
		return ret;
	}
}

PTREE_ITEM find_key_char_child(PTREE_ITEM parent, char *inchar) {
	PTREE_ITEM ret = find_key_char(parent, inchar, 0);
	
	while(ret && ret->parent != parent){
		ret = ret->parent;
	}
	return ret;
}

#ifdef TRIE_TEST

void print_trie_test_res(PTREE_ITEM ret, PTREE_ITEM root){
	PTREE_ITEM res[100] = {0};
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
		fprintf(stdout, "%c", res[nums]->list.l_item.class.name[0]);
	}
	fprintf (stdout, "\n");
}
int main (int argc, char **argv) {
	PTREE_ITEM root = newtreeitem(NULL, "root");
	PTREE_ITEM ret;
// 	char k0[] = "6";
// 	char k1[] = "7";
	
	char k0[] = "6681132122";
	char k1[] = "66811321235";
	char k2[] = "21235";
	char k3[] = "11235";
	char k4[] = "11453435";


	
	insert_key_char(root, k0, 0);
	insert_key_char(root, k1, 0);
	insert_key_char(root, k2, 0);
	insert_key_char(root, k3, 0);
	insert_key_char(root, k4, 0);
	
	root->list.l_item.class.printattributes((PCLASS)root, 0);

	
	ret = find_key_char(root,  k1, 0);
	print_trie_test_res(ret, root);
	
	ret = find_key_char(root, "111", 0);
	print_trie_test_res(ret, root);
	
	ret = find_key_char(root, "11", 0);
	print_trie_test_res(ret, root);
	
}
#endif//TRIE_TEST
