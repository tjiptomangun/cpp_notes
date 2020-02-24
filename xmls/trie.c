#include "parserclass.h"
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

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
PTREE_ITEM insert_child_char(PTREE_ITEM parent, int key, int (*cmp)(char, PTREE_ITEM)) {
	PTREE_ITEM prev = NULL, curr = NULL; 
	char str_item[20] = {0};
	PTREE_ITEM ntr;
	int occ = 1;//assume this key is the first key
	curr = parent->getfirstchild(parent);

	while(curr &&
		(occ = cmp(key, curr))> 0){
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

PTREE_ITEM find_child_char(PTREE_ITEM parent, int key, int (*cmp)(char, PTREE_ITEM)){
	PTREE_ITEM curr = NULL; 
	int occ = -1;//assume this key is the first key
	

	if ((curr = parent->getfirstchild(parent)) == NULL){//we are exhausted by key is not yet found
		return NULL;
	}
	
	while(curr &&
		curr->list.l_item.class.name[0] != 0
		&&
		(occ = cmp(key, curr))> 0){
		curr = parent->getnextchild(parent);
	}
	if (!curr || occ == 0)
		return curr;
	else if (curr->list.l_item.class.name[0] == 0)
		return curr;
	else
		return NULL;
	
	
}

int compare_char(char key, PTREE_ITEM item) {
  char b = item->list.l_item.class.name[0];
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
	if((ret = find_child_char(parent, inchar[idx], compare_char)) && ret->list.l_item.class.name[0]){
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
void find_test(PTREE_ITEM root, char *tofind) {
  fprintf (stderr, "finding %s => ", tofind);
  
	PTREE_ITEM ret = find_key_char(root,  tofind, 0);
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
	PTREE_ITEM root = newtreeitem(NULL, "root"); 
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
	insert_key_char(root, "1013272", 0);
	insert_key_char(root, "011", 0);
	insert_key_char(root, "0342343409", 0);
	insert_key_char(root, "01342343409", 0);
	insert_key_char(root, "01242343409", 0);
	insert_key_char(root, "02242343409", 0);
	insert_key_char(root, "02242343509", 0);
	insert_key_char(root, "0127343509", 0);
	insert_key_char(root, "7127343509", 0);
	insert_key_char(root, "75551509", 0);
	insert_key_char(root, "75552", 0);
	insert_key_char(root, "755522345569", 0);
	insert_key_char(root, "754432343333", 0);
	insert_key_char(root, "784443889348", 0);
	insert_key_char(root, "88444", 0);
	insert_key_char(root, "88555", 0);
	insert_key_char(root, "96555", 0);
	insert_key_char(root, "77555", 0);
	insert_key_char(root, "377555", 0);
	insert_key_char(root, "7439347149414", 0);
	insert_key_char(root, "74847294", 0);
	insert_key_char(root, "27982048274", 0);
	insert_key_char(root, "642403947", 0);
	insert_key_char(root, "3792472928", 0);
	insert_key_char(root, "8374747294", 0);
	insert_key_char(root, "73948274", 0);
	insert_key_char(root, "343549573", 0);
	insert_key_char(root, "43485727", 0);
	insert_key_char(root, "42423947", 0);
	insert_key_char(root, "382", 0);


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

	root->delete(root);
	return 1;	
}
#endif//TRIE_TEST
