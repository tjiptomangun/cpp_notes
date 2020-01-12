#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yxml.h"
#include "xmls.h"

char *xml_find_element(char *xml_string, char *path) {
  return xml_find_attribute(xml_string, path, NULL, NULL, 0);
}

char* xml_find_attribute(char *xml_string, char *path, char *attr_name, char *out_buffer, int outbuffer_max) {
  char *p_path = path;
  char *start_path = NULL, *end_path = NULL;
  int done_path = 0;

  int xml_depth = 0;
  int path_depth = 0;

  char *doc_p = xml_string;
  char *t_doc_p;

  yxml_t xml_elem;
  yxml_ret_t xml_ret;
  char buf[8192];
  yxml_init(&xml_elem, buf, 8192);//2*EC_UTILS_MAX_DB_SIZE
  int match = 0;
  int i = 0;
  int found = 0;
  int ret_len = -1;
  char *out_ptr = NULL;
  char *elem_ptr = NULL;
  char *attrib_ptr = NULL;
  char temp_str[4096] = "";//EC_UTILS_MAX_DB_SIZE

  while (*p_path != 0 && !done_path) {

    while(*p_path == ' '){
      p_path ++;
    }

    start_path = p_path;
    while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
      p_path ++;
    }

    /* empty path or end of string */
    if (start_path == p_path)
      done_path = 1;

    end_path = p_path;

    while (*p_path == ' ')
      p_path ++;

    if (*p_path == '\0')
      done_path = 1;

    if (*p_path == '/')
      p_path ++;

    path_depth ++;

    // find the elem
    found = 0;
    out_ptr = NULL;
    for(; !found && *doc_p; doc_p++) {

        xml_ret = yxml_parse(&xml_elem, *doc_p);
        switch (xml_ret) {
          case YXML_ELEMSTART:
            xml_depth ++;
            int elem_len = strlen(xml_elem.elem);
            int path_len = end_path - start_path;
            match = elem_len == path_len;
            for(i = 0; i < path_len && match; i++)
                match = match && (start_path[i] == xml_elem.elem[i]);

            if (match && xml_depth == path_depth){
              found = 1;
              t_doc_p = doc_p;
              doc_p -= path_len;
              for(;*doc_p != '<'; doc_p--){
                      ;
              }
              elem_ptr = doc_p;
              doc_p = t_doc_p;
            }
            break;
          case YXML_ELEMEND:
            xml_depth --;
            break;

          default:
            break;
        }
    }
    if (found && done_path){
      if (attr_name != NULL){
        int attr_found = 0;
        for(; ret_len < 0 && *doc_p != '\0'; doc_p++){
            xml_ret = yxml_parse(&xml_elem, *doc_p);
            switch (xml_ret) {
            case YXML_ATTRSTART:
                if (!strcmp(attr_name, xml_elem.attr)) {
                    attr_found = 1;
                    attrib_ptr = doc_p - strlen(xml_elem.attr);
                }
                else{
                    attr_found = 0;
                }
                break;
            case YXML_ATTRVAL:
                if (attr_found) {
                  strncat(temp_str, xml_elem.data, 1024);
                }
                break;
            case YXML_ATTREND:
                if (attr_found) {
                    strncpy(out_buffer, temp_str, outbuffer_max);
                    ret_len = strlen(out_buffer);
                    out_ptr = attrib_ptr;
                }
                break;
						case YXML_ELEMEND:
								found = 0;
								done_path = 0;
								return out_ptr;
								break;

            default:
                break;
            }

        }
      }//for
      else {
       out_ptr = elem_ptr;
      }
    }//if
  }//!done_path
  return out_ptr;
}

XML_ELEM_ITERATOR *to_xml_iterator(char *xml_str, char *elem_name, XML_ELEM_ITERATOR *iter) {
	char *tmp;
	char *xptr;
	int len = strlen(elem_name);	
	XML_ELEM_ITERATOR *ret = NULL;
	if (iter == NULL)
		return NULL;
	else if (xml_str == NULL)
		return NULL;
	else if (elem_name == NULL)
		return NULL; 
	else if ((tmp = xml_find_element(xml_str, elem_name)) == NULL)
		return NULL;
	else {
		xptr = &elem_name[len];
		do {
			xptr --;
		}while (*xptr != 0 && *xptr != '/');
		xptr ++;
		iter->src_str = xml_str;
		char *doc_p;
		yxml_t xml_elem;
		yxml_ret_t xml_ret;
		char buf[2048];
		int xml_depth = 0;
		do{	
			yxml_init(&xml_elem, buf, 2048);
			iter->num += 1;
			iter->ptrs[iter->num - 1] = tmp;
			doc_p = tmp; 
			for (; xml_depth == 0; doc_p ++){
				xml_ret = yxml_parse(&xml_elem, *doc_p);
				if (xml_ret == YXML_ELEMSTART)
					xml_depth ++; 
			}
			do {
				xml_ret = yxml_parse(&xml_elem, *doc_p);
				switch(xml_ret) {
					case YXML_ELEMSTART:
						xml_depth ++;
						break;
					case YXML_ELEMEND:
						xml_depth --;
						break;
					default:
						break;
				}
				doc_p ++;
			}while (xml_depth > 0);
		}while ((tmp = xml_find_element(doc_p, xptr)) != NULL);
		ret =  iter; 
	}
	return ret;
}

