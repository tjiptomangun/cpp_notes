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
typedef struct xml_elem_iterator {
  int num; // number of pointer
  char *ptrs[20]; //the pointer collection of start of element
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

#endif //_XMLS_H_
