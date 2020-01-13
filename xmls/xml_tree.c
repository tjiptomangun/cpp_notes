#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yxml.h"
#include "xmls.h"
#include "parserclass.h"

int xml_string_deserialize(char *xml_string){
  char *doc_p = xml_string;
  char *t_doc_p;
  TREE_ITEM *root_tree = newtreeitem(NULL, "root");
  TREE_ITEM *tree_active= root_tree;
  TREE_ITEM *tree_current;

  PROPERTY *prop_active, *prop_current;

  yxml_t xml_elem;
  yxml_ret_t xml_ret;
  char buf[8192];
  yxml_init(&xml_elem, buf, 8192);

  while(doc_p){
    xml_ret = yxml_parse(&xml_elem, *doc_p);
    switch(xml_ret) {
      case YXML_ELEMSTART:
        tree_current = newtreeitem(tree_active, xml_elem.elem);
        tree_active = tree_current;
        prop_current = NULL;
        prop_active = NULL;
        break;

      case YXML_ELEMEND:
        tree_active = tree_active->parent;
        prop_current = NULL;
        prop_active = NULL;
        break;

      case YXML_ATTRSTART:
        prop_current = newproperty(xml_elem.attr);
        tree_active->list.add (&tree_active->list, prop_current); 
        break;

      case YXML_ATTREND:
        break;


      default:

      break;

        
    }
    
    doc_p++;
  }
  return 0;
}
