#include "xmle01.h"
#include "parserclass.h"
#include "yxml.h"
#include <stdlib.h>
#include <string.h>
#include "af.h"

xe01_attribute_data * xe01_attribute_data_ctor(xe01_attribute_data * in, char *name, int (*update_value) (void *, char*), int ptr_only) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_value = update_value;
		in->ptr_only = ptr_only;
		in->ptr_passed = 0;
	}
	return in;
}

xe01_attribute_data * new_xe01_attribute_data(char *name, int (*update_value) (void *, char*), int ptr_only) {
	xe01_attribute_data * ret = (xe01_attribute_data *) calloc(1, sizeof(xe01_attribute_data));
	return xe01_attribute_data_ctor(ret, name, update_value, ptr_only);
}

xe01_attribute_data * xe01_pass_ptr(xe01_attribute_data *in, void * dest, char *inptr)  {
	if (in->ptr_only) {
		if(in->ptr_passed == 0) {
			in->ptr_passed = 1;
			in->update_value(dest, inptr);
		}
	}
	return in;
}

xe01_element_data * xe01_element_data_ctor(xe01_element_data *in, char *name) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
	}
	return in;
}

xe01_element_data * new_xe01_element_data(char *name) {
	xe01_element_data * ret = (xe01_element_data *) calloc(1, sizeof(xe01_element_data));
	return xe01_element_data_ctor(ret, name);
}


int xe01_element_cmp(xe01_element_data * c1, xe01_element_data *c2) {
	return strcmp(c1->name, c2->name);
}

int xe01_attribute_cmp(xe01_element_data * c1, xe01_element_data * c2) {
	return strcmp(c1->name, c2->name);
}

int xe01_do_nothing(void *x, char *in){
	return 1;
}


/**
 * NAME 				: xmlf_add_element
 * DESCRIPTION	: add one element relative to node with path path_to_find. 
 * 								If element already exists will not add or update
 */
PPRIMTREE_ITEM xmle01_add_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	xe01_element_data * elem = NULL;

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
	}

	strncpy(elem_name, start_path, end_path - start_path);
	elem = new_xe01_element_data(elem_name);
	if (! (next_tree = node->find_one(node, elem, (int (*) (void *, void *))xe01_element_cmp))) {
		
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, elem);
		node->add_one(node, next_tree, (int (*) (void *, void *))xe01_element_cmp);
	}
	else {
		free(elem);
	}
	return xmle01_add_element(next_tree, p_path);
	
}

/**
 * _xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: found element
 */
PPRIMTREE_ITEM _xmle01_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	xe01_element_data elem;
	
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
		xe01_element_data_ctor(&elem, elem_name);
		
		if (! (next_tree = node->find_one(node, &elem, (int (*) (void *, void *))xe01_element_cmp))) {
			return next_tree;
		}
		else {
			return _xmle01_find_element(next_tree, p_path);
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
void * xmle01_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	PPRIMTREE_ITEM found = _xmle01_find_element(node, path_to_find);
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
xe01_attribute_data * xmle01_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name){
	PPRIMTREE_ITEM elem = NULL;
	xe01_attribute_data attr;
	PPRIML_ITEM found = NULL;
	xe01_attribute_data_ctor(&attr, attribute_name, xe01_do_nothing, 0);
	if ( node && path_to_find && attribute_name && (elem = _xmle01_find_element(node, path_to_find)) 
		&& (found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))xe01_attribute_cmp)) ) {
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
xe01_attribute_data * xmle01_unset_ptr_passed(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name){
	PPRIMTREE_ITEM elem = NULL;
	xe01_attribute_data attr;
	xe01_attribute_data *fnd;
	
	PPRIML_ITEM found = NULL;
	xe01_attribute_data_ctor(&attr, attribute_name, xe01_do_nothing, 0);
	if ( node && path_to_find && attribute_name && (elem = _xmle01_find_element(node, path_to_find)) 
		&& (found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))xe01_attribute_cmp)) ) {
		fnd = found->get_data(found);
		fnd->ptr_passed = 0;
		return fnd;
	}
	else {
		return NULL;
	}
}

xe01_attribute_data * xmle01_add_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name, xe01_attribute_data *data) {
	PPRIMTREE_ITEM elem = _xmle01_find_element(node, path_to_find);
	if (!elem) {
		elem  = xmle01_add_element(node, path_to_find);
	}
	xe01_attribute_data attr;
	xe01_attribute_data_ctor(&attr, attribute_name, xe01_do_nothing, 0);
	PPRIML_ITEM found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))xe01_attribute_cmp);
	if (!found) {
			PPRIML_ITEM new_item = newpriml_item();
			new_item->set_data(new_item, data);
			new_item->set_data_remove_fn(new_item, (int (*) (void *)) free);
			elem->list.add_one(&elem->list, new_item, (int (*) (void *, void *))xe01_attribute_cmp);
	}
	else {
		found->data_remove_fn(found);
		found->set_data(found, data);
		found->set_data_remove_fn(found, (int (*) (void *)) free);
	}
	return data;
}


int xmle01_parsedata(char *xml_string, PRIMTREE_ITEM *fn_data, void * dest) {
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
	PDLIST_ITEM dlitem;
	xe01_attribute_data *attdat = NULL;
	int ptr_passed = 0;
	xmle01_unset_ptr_passed(fn_data, "message/data", "value");
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
				item_path[0] = '\0';
				dlitem = stack.pop(&stack);
				dlitem->delete(dlitem);
				dlitem = NULL;
				break;

			case YXML_ATTRSTART:
				strcpy(attrib_name, xml_elem.attr);
				dlist_serialize(&stack, item_path, "/");
				attdat = xmle01_find_attribute(fn_data, item_path, attrib_name);
				ptr_passed = 0;
				break;
			case YXML_ATTRVAL:
				if (attdat)  {
					if (attdat->ptr_only) {
						if (!ptr_passed){
							xe01_pass_ptr(attdat, dest, doc_p);
							ptr_passed = 1;
						}
					}
					else {
						strcat(attrib_val, xml_elem.data);
					}
				}

				break;
			case YXML_ATTREND:
				dlist_serialize(&stack, item_path, "/");
				if (attdat)  {
					if (!attdat->ptr_only) {
						attdat->update_value(dest, attrib_val);
					}
					else {
						*doc_p = 0;
					}
				}
				attrib_name [0] = '\0';
				attrib_val [0] = '\0';
				item_path[0] = '\0';
				break;
			case YXML_CONTENT:
				item_path[0] = '\0';
				break;
			case YXML_ESYN:
				return -1;
			default: 
				break;
		}
		doc_p ++;
	}
	return 1;
}


