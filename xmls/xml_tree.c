/**
 * author : henky <hanky.acbb@telogic.com.sg>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yxml.h"
#include "parserclass.h" 

/**
 * NAME					: xml_string_deserialize
 * DESCRIPTION	: deserialize an xml string to our internal structure
 * INPUT
 * 	xml_string	: string to deserialize
 * 	root_tree		: storage
 */
int xml_string_deserialize(char *xml_string, TREE_ITEM *root_tree){
	char *doc_p = xml_string;
	TREE_ITEM *tree_active= root_tree;
	TREE_ITEM *tree_current = NULL;

	PROPERTY *prop_active = NULL;

	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	char tmp[1024] = {0};
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				tree_current = newtreeitem(tree_active, xml_elem.elem);
				tree_active->add(tree_active, tree_current);
				tree_active = tree_current;
				prop_active = NULL;
				break;

			case YXML_ELEMEND:
				tree_active = tree_active->parent;
				prop_active = NULL;
				break;

			case YXML_ATTRSTART:
				prop_active= newproperty(xml_elem.attr);
				tree_active->list.add (&tree_active->list, (L_ITEM*)prop_active); 
				break;

			case YXML_ATTRVAL:
				strcat(tmp, xml_elem.data);
				break;
			case YXML_ATTREND:
				prop_active->setvalue(prop_active, tmp); 
				tmp[0] = 0;
				prop_active = NULL;
				break;
			case YXML_ESYN:
				return -1;
			default: 
				break; 
		}
		
		doc_p++;
	}
	return 0;
}

/**
 * NAME					: xml_string_deserialize_root
 * DESCRIPTION	: deserialize an xml string to our internal structure, 
 *                this xml probably have several top level element
 *                which is non standard
 * INPUT
 * 	xml_string	: string to deserialize
 * 	root_tree		: storage
 */
int xml_string_deserialize_multiroot(char *xml_string, TREE_ITEM *root_tree){
	char *doc_p = xml_string;
	TREE_ITEM *tree_active= root_tree;
	TREE_ITEM *tree_current;

	PROPERTY *prop_active;

	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	char tmp[1024] = {0};
	int i = 0;
	int depth = 0;

	//yxml_selfclose
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				tree_current = newtreeitem(tree_active, xml_elem.elem);
				tree_active->add(tree_active, tree_current);
				tree_active = tree_current;
				prop_active = NULL;
				depth += 1;
				break;

			case YXML_ELEMEND:
				tree_active = tree_active->parent;
				prop_active = NULL;
				depth -= 1;
				break;

			case YXML_ATTRSTART:
				prop_active= newproperty(xml_elem.attr);
				tree_active->list.add (&tree_active->list, (L_ITEM*)prop_active); 
				break;

			case YXML_ATTRVAL:
				strcat(tmp, xml_elem.data);
				break;

			case YXML_ATTREND:
				prop_active->setvalue(prop_active, tmp); 
				tmp[0] = 0;
				prop_active = NULL;
				break; 
			case YXML_ESYN:
				// 34 is yxml.state YXMLS_le3 , see yxml.c enum yxml_state_t
				if (depth == 0 && doc_p != xml_string && xml_elem.state == 34) {
					while(*doc_p != '<') {
						doc_p -= 1;
					}
					yxml_init(&xml_elem, buf, 8192);
					continue;
				}
				else {
					return -1;
				}
				break;
			default: 
				i++;
			break; 
		}
		
		doc_p++;
	}
	return 0;
}

/**
 * NAME					: xml_tree_find_element
 * DESCRIPTION	: find an element in a tree
 * INPUT	
 * 	root_tree		: container
 * 	path_to_find: the path in format elem/elem/elem example SS7AP/SCCP/CDPA
 * RETURNS
 * 	pointer of element in tree
 */
TREE_ITEM* xml_tree_find_element(TREE_ITEM *root_tree, char *path_to_find) {
	
  char *p_path = path_to_find;
  char *start_path = NULL, *end_path = NULL;

  char elem_name[100] = {0};
  TREE_ITEM *next_tree = NULL;
  
    while(*p_path == ' '){
      p_path ++;
    }

    start_path = p_path;
    while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
      p_path ++;
    }

    /* empty path or end of string */
    if (start_path == p_path){
	  return root_tree;
	}

    end_path = p_path;

    while (*p_path == ' ')
      p_path ++;

    if (*p_path == '/')
      p_path ++;
	
	strncpy(elem_name, start_path, end_path - start_path);
	
	next_tree = root_tree->getname(root_tree, elem_name);
	
	if (!next_tree)
		return next_tree;
	else
		return xml_tree_find_element(next_tree, p_path);
}

/**
 * NAME						: xml_tree_delete_element
 * DESCRIPTION		: delete tree element
 * RETURNS
 * 						1		: found and deleted
 * 						0		: not found
 */
int xml_tree_delete_element(TREE_ITEM *root_tree, char *path) {
	
	TREE_ITEM *to_del;
	if ((to_del = xml_tree_find_element(root_tree, path))) {
		TREE_ITEM *parent = to_del->parent;
		if (!parent->detach(parent , to_del)) {
			to_del->delete(to_del);
		}
		return 1;
	}
	return 0;
}

/**
 * NAME					: xml_tree_find_attribute
 * DESCRIPTION	: find an attribute
 * INPUT
 * 	root_tree		: container
 * 	path				: path to element that contains this in format elem/elem/elem example SS7AP/TCAP/COMPONENT/DATA
 * 	attrib_name	: name of the attribute example value
 * RETURNS
 * 	pointer to attribute
 */
PROPERTY *xml_tree_find_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name) {
	TREE_ITEM *elem = NULL;
	PROPERTY *ret = NULL;
	
	if ((elem = xml_tree_find_element(root_tree, path)) != NULL) {
		ret = (PPROPERTY) elem->list.getname(&elem->list, attrib_name);
	}
	return ret;
}

char *xml_tree_get_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name, char *output, size_t max_size) {
	TREE_ITEM *elem = NULL;
	PROPERTY *ret = NULL;
	if (!root_tree) {
    return NULL;
  }
	else if ((elem = xml_tree_find_element(root_tree, path)) != NULL) {
		ret = (PPROPERTY) elem->list.getname(&elem->list, attrib_name);
		if (!ret)
			return NULL;
		else {
			ret->getvalue(ret, output, max_size);
		}
	}
	else {
		return NULL;
	}
	return output;
}


/**
 * NAME						: xml_tree_set_attribute
 * DESCRIPTION		: set an xml attribute value
  * INPUT
 * 	root_tree			: container
 * 	path					: path to element that contains this in format elem/elem/elem example SS7AP/TCAP/COMPONENT/DATA
 * 	attrib_name		: name of the attribute example value
 * 	new value			: new element value
 * RETURNS
 * 	1							: success
 * 	0							: element or attribute name does not exists
 */
int xml_tree_set_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name, char *new_value) {
	PROPERTY *ret = NULL;
	if ((ret = xml_tree_find_attribute(root_tree, path, attrib_name)) != NULL){
		ret->setvalue(ret, new_value);
		return 1 ;
	}
	return 0;
}

int xml_tree_delete_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name) {
	PROPERTY *ret = NULL;
	if ((ret = xml_tree_find_attribute(root_tree, path, attrib_name)) != NULL){
		ret->l_item.class.delete((CLASS*)ret);
		return 1 ;
	}
	return 0;
}

/**
 * NAME						: xml_tree_serialize
 * DESCRIPTION		: serialize xml tree to string
 * INPUT
 * 	root_tree			: container
 * 	outbuf				: string result
 * 	outmax				: maximum size
 * 	currlen				: current string length
 * RETURNS
 * 	i							: current length
 */
int xml_tree_serialize(TREE_ITEM *root_tree, char *outbuf, int outmax, int curr_len) {
	int i = curr_len;
	int j;
	TREE_ITEM *tcurr = NULL;
	PROPERTY *lcurr = NULL;
	if (!root_tree)
		return 0;
	LIST *list = &root_tree->list;
	outbuf[i++] = '<';
	for (j = 0; list && list->l_item.class.name[j] != 0; j++, i++){
		outbuf[i] = list->l_item.class.name[j];
	}
	
	lcurr = (PROPERTY *)list->getfirstchild(list);
	if (lcurr)
		outbuf[i++] = ' ';

	while(lcurr){
		for(j = 0; lcurr->l_item.class.name[j]; j++, i++){
			outbuf[i] = lcurr->l_item.class.name[j];
		}
		
		outbuf[i++] = '=';
		
		outbuf[i++] = '"';
		char *pv = lcurr->getvalue_ptr(lcurr);
		for(j = 0; pv[j]; j++, i++){
			outbuf[i] = pv[j];
		}
		outbuf[i++] = '"';
		
		outbuf[i++] = ' ';
		lcurr = (PROPERTY *)list->getnextchild(list);
		
	}
	outbuf[i++] = '>';
	
	tcurr = root_tree->getfirstchild(root_tree);
	
	while(tcurr){
		i = xml_tree_serialize(tcurr, outbuf, outmax, i);
		tcurr = root_tree->getnextchild(root_tree);
	}
	
	outbuf[i++] = '<';
	outbuf[i++] = '/';
	for (j = 0; list && list->l_item.class.name[j] != 0; j++, i++){
		outbuf[i] = list->l_item.class.name[j];
	}
	outbuf[i++] = '>';
	if (curr_len == 0)
		outbuf[i++] = 0;
	
	return i;
}
