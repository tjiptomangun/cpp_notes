#include "xmlf.h"
#include "parserclass.h"
#include "radixtree.h"
#include "yxml.h"
#include <stdlib.h>
#include <string.h>

xfn_attribute_data * xfn_attribute_data_ctor(xfn_attribute_data * in, char *name, int (*update_value) (void*, char*), void *dest) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_value = update_value;
		in->dst_ptr = dest;
	}
	return in;
}

xfn_attribute_data * new_xfn_attribute_data(char *name, int (*update_value) (void *, char*), void *dest) {
	xfn_attribute_data * ret = (xfn_attribute_data *) calloc(1, sizeof(xfn_attribute_data));
	return xfn_attribute_data_ctor(ret, name, update_value, dest);
}

xfn_element_data * xfn_element_data_ctor(xfn_element_data *in, char *name, int (*update_value) (void*, char*), int extract_content, void *dest) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_content = update_value;
		in->extract_content = extract_content;
		in->dst_ptr = dest;
	}
	return in;
}

xfn_element_data * new_xfn_element_data(char *name, int (*update_value) (void *, char*), int extract_content, void *dest) {
	xfn_element_data * ret = (xfn_element_data *) calloc(1, sizeof(xfn_element_data));
	return xfn_element_data_ctor(ret, name, update_value, extract_content, dest);
}


int xfn_element_cmp(xfn_element_data * c1, xfn_element_data *c2) {
	return strcmp(c1->name, c2->name);
}

int xfn_attribute_cmp(xfn_attribute_data * c1, xfn_attribute_data * c2) {
	return strcmp(c1->name, c2->name);
}

int xfn_do_nothing(void *dest, char *in){
	return 1;
}

/**
 * NAME 				: xmlf_add_element
 * DESCRIPTION	: add one element relative to node with path path_to_find. 
 * 								If element already exists will not add or update
 */
PPRIMTREE_ITEM xmlf_add_element(PPRIMTREE_ITEM node, char *path_to_find, int (*update_value) (void *, char*), void *dest){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	xfn_element_data * elem = NULL;
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
		elem = new_xfn_element_data(elem_name, xfn_do_nothing, 0, NULL);
	}
	else {
		elem = new_xfn_element_data(elem_name, update_value, 1, dest);
	}
	if (! (next_tree = node->find_one(node, elem, (int (*) (void *, void *))xfn_element_cmp))) {
		
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, elem);
		node->add_one(node, next_tree, (int (*) (void *, void *))xfn_element_cmp);
	}
	else {
		free(elem);
	}
	return xmlf_add_element(next_tree, p_path, update_value, dest);
	
}

/**
 * _xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: found element
 */
PPRIMTREE_ITEM _xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	xfn_element_data elem;
	
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
		xfn_element_data_ctor(&elem, elem_name, xfn_do_nothing, 0, NULL);
		
		if (! (next_tree = node->find_one(node, &elem, (int (*) (void *, void *))xfn_element_cmp))) {
			return next_tree;
		}
		else {
			return _xmlf_find_element(next_tree, p_path);
		}
}

/**
 * xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: data that stored in this element
 */
void * xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	PPRIMTREE_ITEM found = _xmlf_find_element(node, path_to_find);
	if(found) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}
/**
 * NAME				 	: xmlf_find_attribute
 * DESCRIPTION 	: find attribute data attached to path_to_find attribute_name
 * 								this data is previously allocated
 */
xfn_attribute_data * xmlf_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name){
	PPRIMTREE_ITEM elem = NULL;
	xfn_attribute_data attr;
	PPRIML_ITEM found = NULL;
	xfn_attribute_data_ctor(&attr, attribute_name, xfn_do_nothing, NULL);
	if ( node && path_to_find && attribute_name && (elem = _xmlf_find_element(node, path_to_find)) 
		&& (found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))xfn_attribute_cmp)) ) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}
