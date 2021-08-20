#include "xmlfunc.h"
#include "parserclass.h"
#include "yxml.h"
#include <stdlib.h>
#include <string.h>
#include "af.h"

PXMLFUNC xmlfunc_get(PXMLFUNC fn_data, int xmlfunc_max, char *path, char *attrib) {
	for (int i = 0; i < xmlfunc_max; i++) {
		if (!strcmp(fn_data[i].path, path) && !strcmp(fn_data[i].attribute, attrib)) {
			return &fn_data[i];
		}
	}
	return NULL;
}

PXMLFUNC xmlfunc_pass_ptr(PXMLFUNC in, void * dest, char *inptr)  {
	if (in->ptr_only) {
		if(in->ptr_passed == 0) {
			in->ptr_passed = 1;
			in->update_value(dest, inptr);
		}
	}
	return in;
}

PXMLFUNC xmlfunc_unset_ptr_passed(PXMLFUNC in, int xmlfunc_max, char *path_to_find, char *attribute)  {
	PXMLFUNC found = NULL;
	if ((found = xmlfunc_get(in, xmlfunc_max, path_to_find, attribute))) {
		found->ptr_passed = 0;
		return found;
	}
	else {
		return NULL;
	}
	
}

int xmlfunc_parsedata(char *xml_string, PXMLFUNC fn_data, int xmlfunc_max, void * dest) {
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
	char attrib_val[4092] = {0};
	char content_val[4092] = {0};
	PDLIST_ITEM dlitem;
	PXMLFUNC attdat = NULL;
	PXMLFUNC contdat = NULL;
	int (*update_content) (void *, char *);
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				dlitem = new_dlist_item();
				item_name = copy_string(&item_name, xml_elem.elem);
				dlitem->set_data(dlitem, item_name);
				stack.push(&stack, dlitem);
				item_name = NULL;
				dlist_serialize(&stack, item_path, "/");
 				contdat = xmlfunc_get(fn_data, xmlfunc_max, item_path, "");
				if(contdat) {
					update_content = contdat->update_content;
				}
				else {
					update_content = NULL;
				}
				item_path[0] = 0;
				content_val[0] = 0;
				break;

			case YXML_ELEMEND:
				item_path[0] = '\0';
				dlitem = stack.pop(&stack);
				dlitem->delete(dlitem);
				dlitem = NULL;
				if (update_content && content_val[0])  {
					update_content(dest, content_val);
				}
				content_val[0] = 0;
				update_content = NULL;
				break;

			case YXML_ATTRSTART:
				strcpy(attrib_name, xml_elem.attr);
				dlist_serialize(&stack, item_path, "/");
				attdat = xmlfunc_get(fn_data,xmlfunc_max ,item_path, attrib_name);
				item_path[0] = '\0';
				break;
			case YXML_ATTRVAL:
				if (attdat)  {
					if (attdat->ptr_only) {
						if (!attdat->ptr_passed) {
							xmlfunc_pass_ptr(attdat, dest, doc_p);
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
					if (!attdat->ptr_only && attdat->update_value) {
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
				if (update_content)  {
					strcat(content_val, xml_elem.data);
				}
				
				break;
			case YXML_ESYN:
				dlist_cleanup(&stack);
				return -1;
			default: 
				break;
		}
		doc_p ++;
	}
	dlist_cleanup(&stack);
	return 1;
}
