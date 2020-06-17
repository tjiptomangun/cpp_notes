#ifndef _XMLS_H_
#define _XMLS_H_

/**
 * NAME         : xml_find_element
 * DESCRIPTION  : find element of a an xml string. Returns to pointer of this element
 * INPUT
 *  xml_string  : xml string to find element from
 *  path to find: the path to search for.
 *                example  "SS7AP / SCCP / CDPA / GTITLE" will look
 *                for GTITLE within CDPA within SCCP within SS7AP
 * RETURNS
 *      NULL    : path does not is_exists
 *      others  : pointer to element
 *
 */

char *xml_find_element(char *xml_string, char *path);
/**
 * NAME         : xml_find_attribute
 * DESCRIPTION  : get an attribute's value of xml path from an xml string.
 * INPUT
 *  xml_string  : xml string to find element from
 *  path to find: the path to search for.
 *                example  "SS7AP / SCCP / CDPA / GTITLE" will look
 *                for GTITLE within CDPA within SCCP within SS7AP
 *  attribute   : name of the attributes get value from within path_to_find.
 *                if null then will return pointer to element and no value copied.
 *
 *  out_buf_size: size of output buffer to store attribute value
 * OUTPUT
 *  outbuf      : value to copy the attribute value to
 *
 * RETURNS
 *   NULL       : path/attribute does not is_exists
 *   others     : pointer to the attribute
 *
 */
char *xml_find_attribute(char *xml_string, char *path, char *attr_name, char *out_buffer, int outbuffer_max);

/**
 * collection of pointers to xml element of same name
 */
#define ELEM_ITER_MAX 80
typedef struct xml_elem_iterator {
  int num; // number of pointer
  char *ptrs[ELEM_ITER_MAX]; //the pointer collection of start of element
  char *src_str; // source xml string
}XML_ELEM_ITERATOR;

/**
 * NAME          : to_xml_iterator
 * DESCRIPTION   : create element iterator from an xml string
 * INPUT
 * 	xml_str      : string to find iterator from
 *  elem_name    : name of element to iterate 
 *  iter         : iterator buffer
 *
 * RETURNS
 *  NULL		 : none exists
 *  iter		 : found some
 */
XML_ELEM_ITERATOR *to_xml_iterator(char *xml_str, char *elem_name, XML_ELEM_ITERATOR *iter);

/**
 * NAME           : xml_foreach_element
 * DESCRIPTION    : do function fn if start of element found
 * INPUT
 *      xml_str   : xml element to yxml_parse
 *      elem_name : element name to find for
 *      fn        : function that process found element
 * 
 * RETURNS        : number of element found 
 * 
 */
int xml_foreach_element(char *xml_str, char *elem_name, int (*fn)(char *));

/**
 * NAME									: xml_find_content
 * DESCRIPTION					: get content or value attibutes value of an xml element
 * INPUT
 * 			xml_String			: source xml string 
 * 			path 						: path to element
 *OUTPUT 
 *			out_buffer			: storage of found data
 * 			outbuffer_max		: maximum length of output buffer
 */
char* xml_find_content(char *xml_string, char *path, char *out_buffer, int outbuffer_max);


/**
 * NAME							: xml_collect_elements
 * DESCRIPTION			: collect information from elements
 * INPUT
 * 			xml_str 		: source xml string
 * 			elem_name		: name of the element
 * 			fn 					: a function that will process found xml element and collect data
 * 									: the first argument of this function is xml string of element found
 * 									: the second is callers data
 * 									: collected is collected data
 */
int xml_collect_elements(char *xml_str, char *elem_name, int (*fn)(char *, void *), void *collected);
#endif //_XMLS_H_
