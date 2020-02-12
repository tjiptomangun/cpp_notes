#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yxml.h"
#include "xmls.h"
#include "parserclass.h" 
int xml_string_deserialize(char *xml_string, TREE_ITEM *root_tree){
  char *doc_p = xml_string;
  TREE_ITEM *tree_active= root_tree;
  TREE_ITEM *tree_current;

  PROPERTY *prop_active;

  yxml_t xml_elem;
  yxml_ret_t xml_ret;
  char buf[8192] = {0};
  yxml_init(&xml_elem, buf, 8192);
  char tmp[48] = {0};

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

      default: 
		break; 
    }
    
    doc_p++;
  }
  return 0;
}

TREE_ITEM* xml_tree_find_element(TREE_ITEM *root_tree, char *path_to_find) {
	
  char *p_path = path_to_find;
  char *start_path = NULL, *end_path = NULL;

  char elem_name[100] = {0};
  TREE_ITEM *next_tree;
  
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


PROPERTY *xml_tree_find_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name) {
	TREE_ITEM *elem = NULL;
	PROPERTY *ret = NULL;
	
	if ((elem = xml_tree_find_element(root_tree, path)) != NULL) {
		ret = (PPROPERTY) elem->list.getname(&elem->list, attrib_name);
	}
	return ret;
}


int xml_tree_serialize(TREE_ITEM *root_tree, char *outbuf, int outmax, int curr_len) {
	int i = curr_len;
	int j;
	TREE_ITEM *tcurr;
	PROPERTY *lcurr;
	LIST *list = &root_tree->list;
	outbuf[i++] = '<';
	for (j = 0; list->l_item.class.name[j] != 0; j++, i++){
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
	for (j = 0; list->l_item.class.name[j] != 0; j++, i++){
		outbuf[i] = list->l_item.class.name[j];
	}
	outbuf[i++] = '>';
	
	return i;
}
