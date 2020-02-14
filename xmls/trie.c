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
PTREE_ITEM insert_child_int(PTREE_ITEM parent, int key, int (*cmp)(int, int)) {
	PTREE_ITEM prev = NULL, curr = NULL; 
	char str_item[20] = {0};
	PTREE_ITEM ntr;
	int occ = -1;//assume this key occur after first key
	curr = parent->getfirstchild(parent);

	while(curr &&
		(occ = cmp(key, atoi(curr->list.l_item.class.name)))< 0){
		prev = curr;
		curr = parent->getnextchild(parent);
	}

	if (occ == 0) {
		return curr;	
	}

	sprintf(str_item, "%d", key);
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

int compare_int(int key, int b) {
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
PTREE_ITEM insert_key_int(PTREE_ITEM parent, char *inchar, int idx) {
	PTREE_ITEM ret;
	if((ret = insert_child_int(parent, inchar[idx], compare_int)) && inchar[idx]!=0){
		return insert_key_int(ret, inchar, ++idx);
	}
	else{
		return ret;
	}
}

#ifdef TRIE_TEST
int main (int argc, char **argv) {
	PTREE_ITEM root = newtreeitem(NULL, "root");
	PTREE_ITEM p0, p1, p2, p3, p4;
	char k0[] = "6681132122";
	char k1[] = "66811321235";
	char k2[] = "21235";
	char k3[] = "11235";
	char k4[] = "11453435";

	p0 = insert_key_int(root, k0, 0);
	p1 = insert_key_int(root, k1, 0);
	p2 = insert_key_int(root, k2, 0);
	p3 = insert_key_int(root, k3, 0);
	p4 = insert_key_int(root, k4, 0);

	root->list.l_item.class.printattributes((PCLASS)&root, 0);
	
}
#endif//TRIE_TEST
