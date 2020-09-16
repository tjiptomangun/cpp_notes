#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "radixtree.h"
#include "yxml.h"

typedef struct {
	char statResetInterval[20];
	char httpMaxSession[20];
	PRADIXTREE_ITEM ussdSpamBlackTree;
	PRADIXTREE_ITEM smsByPassRoamTree;
} af_config;

typedef struct {
	char name [20];
	int (*update_content) (char*);
	int extract_content;
}element_data;


typedef struct {
	char name [20];
	int (*update_value) (char*);
}attribute_data;

attribute_data * attribute_data_ctor(attribute_data * in, char *name, int (*update_value) (char*)) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_value = update_value;
	}
	return in;
}

attribute_data * new_attribute_data(char *name, int (*update_value) (char*)) {
	attribute_data * ret = (attribute_data *) calloc(1, sizeof(attribute_data));
	return attribute_data_ctor(ret, name, update_value);
}

element_data * element_data_ctor(element_data *in, char *name, int (*update_value) (char*), int extract_content) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_content = update_value;
		in->extract_content = extract_content;
	}
	return in;
}

element_data * new_element_data(char *name, int (*update_value) (char*), int extract_content) {
	element_data * ret = (element_data *) calloc(1, sizeof(element_data));
	return element_data_ctor(ret, name, update_value, extract_content);
}

af_config config;

int updateResetInterval(char *in_value) {
	strcpy(config.statResetInterval, in_value);
	return 1;
}

int updateHttpMaxSession(char *in_value) {
	strcpy(config.httpMaxSession, in_value);
	return 1;
}

int addUssdSpamBlackTree(char *in_value) {
	radixtreeitem_insertkey(config.ussdSpamBlackTree, in_value);
	return 1;
}

int addSmsByPassRoamTree(char *in_value) {
	radixtreeitem_insertkey(config.smsByPassRoamTree, in_value);
	return 1;
}

int element_cmp(element_data * c1, element_data *c2) {
	return strcmp(c1->name, c2->name);
}

int attribute_cmp(attribute_data * c1, attribute_data * c2) {
	return strcmp(c1->name, c2->name);
}

int do_nothing(char *in){
	return 1;
}


PPRIMTREE_ITEM xmlf_add_element(PPRIMTREE_ITEM node, char *path_to_find, int (*update_value) (char*)){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	element_data * elem = NULL;
	int have_next = 0;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

	while(*p_path == ' '){
		p_path ++;
	}

	start_path = p_path;
	while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
		p_path ++;
	}

	/* empty path or end of string */
	if (start_path == p_path){
		return node;
	}

	end_path = p_path;

	while (*p_path == ' ')
		p_path ++;

	if (*p_path == '/') {
		p_path ++;
		have_next = 1;
	}

	strncpy(elem_name, start_path, end_path - start_path);
	if (have_next) {
		elem = new_element_data(elem_name, do_nothing, 0);
	}
	else {
		elem = new_element_data(elem_name, update_value, 1);
	}
	if (! (next_tree = node->find_one(node, elem, (int (*) (void *, void *))element_cmp))) {
		
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, elem);
		node->add_one(node, next_tree, (int (*) (void *, void *))element_cmp);
	}
	else {
		free(elem);
	}
	return xmlf_add_element(next_tree, p_path, update_value);
	
}

PPRIMTREE_ITEM _xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	element_data elem;
	
	PPRIMTREE_ITEM next_tree;

		while(*p_path == ' '){
			p_path ++;
		}

		start_path = p_path;
		while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
			p_path ++;
		}

		/* empty path or end of string */
		if (start_path == p_path){
			return node;
		}

		end_path = p_path;

		while (*p_path == ' ')
			p_path ++;

		if (*p_path == '/')
			p_path ++;

		strncpy(elem_name, start_path, end_path - start_path);
		element_data_ctor(&elem, elem_name, do_nothing, 0);
		
		if (! (next_tree = node->find_one(node, &elem, (int (*) (void *, void *))element_cmp))) {
			return next_tree;
		}
		else {
			return _xmlf_find_element(next_tree, p_path);
		}
}


void * xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	PPRIMTREE_ITEM found = _xmlf_find_element(node, path_to_find);
	if(found) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}

attribute_data * xmlf_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name){
	PPRIMTREE_ITEM elem = NULL;
	attribute_data attr;
	PPRIML_ITEM found = NULL;
	attribute_data_ctor(&attr, attribute_name, do_nothing);
	if ( node && path_to_find && attribute_name && (elem = _xmlf_find_element(node, path_to_find)) 
		&& (found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))attribute_cmp)) ) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}

int setup_functions(PPRIMTREE_ITEM root) {
	PPRIMTREE_ITEM tmp = NULL;
	xmlf_add_element(root, "configuration/warm/StatResetInterval", updateResetInterval);
	xmlf_add_element(root, "configuration/warm/HttpMaxSession", updateHttpMaxSession);
	xmlf_add_element(root, "configuration/warm/ussdSpamBlacklist", addUssdSpamBlackTree);
	xmlf_add_element(root, "configuration/warm/smsByPassRoaming", addSmsByPassRoamTree);
	if ((tmp = _xmlf_find_element(root, "configuration/warm/StatResetInterval"))) {
		attribute_data *udata = new_attribute_data("value", updateResetInterval);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/HttpMaxSession"))) {
		attribute_data *udata = new_attribute_data("value", updateHttpMaxSession);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/ussdSpamBlacklist"))) {
		attribute_data *udata = new_attribute_data("value", addUssdSpamBlackTree);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/smsByPassRoaming"))) {
		attribute_data *udata = new_attribute_data("value", addSmsByPassRoamTree);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	return 1;
}

PPRIMTREE_ITEM xmlf_find_one_child(PPRIMTREE_ITEM item, char *name) {
	PPRIMTREE_ITEM found = NULL;
	element_data to_find;
	element_data_ctor(&to_find, name, do_nothing, 0);

	if (item && ( (found = item->find_one(item, &to_find,  (int (*) (void *, void *))element_cmp)) ) ){
		return found;
	}
	return NULL;
}

char *dlist_serialize(PDLIST dlist, char *out, char *delimiter) {
	if (dlist) {
		PDLIST_ITEM curr = dlist->head;
		while(curr) {
			if (curr->data){
				strcat(out, (char *) curr->data);
				if(curr->next) {
					strcat(out, delimiter);
				}
			}
			curr = curr->next;
		}
		return out;
	}
	else {
		return NULL;
	}
}

int xmlf_parseconfig(char *xml_string, PRIMTREE_ITEM *fn_data) {
	char *doc_p = xml_string;
	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	DLIST stack;
	dlist_ctor(&stack);
	char *item_name = NULL;
	char item_path[200] = {0};
	char attrib_name[200] = {0};
	char attrib_val[200] = {0};
	char content_val[200] = {0};
	PDLIST_ITEM dlitem;
	element_data *eldat = NULL;
	attribute_data *attdat = NULL;
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				dlitem = new_dlist_item();
				item_name = copy_string(&item_name, xml_elem.elem);
				dlitem->set_data(dlitem, item_name);
				stack.push(&stack, dlitem);
				item_name = NULL;
				break;
			case YXML_ELEMEND:
				dlist_serialize(&stack, item_path, "/");
				if ((eldat = xmlf_find_element(fn_data, item_path)) && eldat->extract_content && content_val[0]) {
					eldat->update_content(content_val);
				}
				content_val[0] = '\0';
				item_path[0] = '\0';
				dlitem = stack.pop(&stack);
				dlitem->delete(dlitem);
				dlitem = NULL;
				break;

			case YXML_ATTRSTART:
				strcpy(attrib_name, xml_elem.attr);
				break;
			case YXML_ATTRVAL:
				strcat(attrib_val, xml_elem.data);
				break;
			case YXML_ATTREND:
				dlist_serialize(&stack, item_path, "/");
				if ( (attdat = xmlf_find_attribute(fn_data, item_path, attrib_name)) ) {
					attdat->update_value(attrib_val);
				}
				attrib_name [0] = '\0';
				attrib_val [0] = '\0';
				item_path[0] = '\0';
				break;
			case YXML_CONTENT:
				dlist_serialize(&stack, item_path, "/");
				if ((eldat = xmlf_find_element(fn_data, item_path)) && eldat->extract_content) {
					strcat(content_val, xml_elem.data);
				}
				item_path[0] = '\0';
				break;
			default: 
				break;
		}
		doc_p ++;
	}
	return 1;
}

int file_size(FILE *fin) {
  int nPos,nFSize;
  nPos=ftell(fin);
  fseek(fin,0L,SEEK_END);
  nFSize=ftell(fin);
  fseek(fin,nPos,SEEK_SET);
  return nFSize;
}

#include <unistd.h>

#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

void print_assertion(int assertion) {
  if (assertion){
    printf("\n%sOk", KBLU);
  }
  else{
    printf("\n%sfailed", KRED);
  }
  printf("%s\n", KNRM);
}

void test_fun(char *name, int fn){
	fprintf(stdout, "%s+%s\n", KCYN, name); 
	printf("%s", KNRM);  
	print_assertion(fn);
}


int main (int argc, char **argv) {
	
	if (argc < 2) {
		fprintf(stdout , "please %s xml_file.xml\n", argv[0]);
		return 1;
	}
	
	do{
		ssize_t n_read = 0;
		ssize_t n_buff = 0;
		PRIMTREE_ITEM root;
		PPRIMTREE_ITEM curr;
		primtreeitem_ctor(&root);
		setup_functions(&root);
		
		config.ussdSpamBlackTree = new_radixtreeitem(NULL, 0, NULL);
		config.smsByPassRoamTree = new_radixtreeitem(NULL, 0, NULL);
		FILE *fp = fopen(argv[1], "r");
		int fsz = file_size(fp);
		char * holder = calloc(fsz + 1, sizeof(char));
		char *ptrh = holder;    
		do {
			ptrh+=n_read;
			n_read = fread(ptrh, 1, 32, fp);
			n_buff += n_read;
			//fprintf(stdout, "n_read = %zd n_buff = %zd\n", n_read, n_buff);
		}while(n_read == 32 && n_buff < fsz);
		
		xmlf_parseconfig(holder, &root);
			
		test_fun("statResetInterval [1440]", !strcmp(config.statResetInterval, "1440"));
		test_fun("HttpMaxSession [100]", !strcmp(config.httpMaxSession, "100"));
		test_fun("ussdSpamBlackTree present [856207000048]", radixtreeitem_findkey(config.ussdSpamBlackTree, "856207000048") != NULL);
		test_fun("ussdSpamBlackTree absent  [342342323]", !radixtreeitem_findkey(config.ussdSpamBlackTree, "342342323"));
		test_fun("smsByPassRoamTree present [66810000015]", radixtreeitem_findkey(config.smsByPassRoamTree, "66810000015") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(config.smsByPassRoamTree, "66818124579"));
		fclose(fp);
		free(holder);
		curr = root.detach_head(&root) ;
		while(curr) {
			curr->delete(curr);
			curr = root.detach_head(&root) ;
		}
		radixtreeitem_deletenode(config.ussdSpamBlackTree);
		radixtreeitem_deletenode(config.smsByPassRoamTree);
		sleep(1);
	}while(argc >= 3 && !strcmp(argv[2], "-m"));
}
