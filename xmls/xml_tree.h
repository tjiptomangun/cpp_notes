#ifndef _XML_TREE_H_
#define _XML_TREE_H_
#include "parserclass.h"

/**
 * NAME					: xml_string_deserialize
 * DESCRIPTION	: deserialize an xml string to our internal structure
 * INPUT
 * 	xml_string	: string to deserialize
 * 	root_tree		: storage
 */
int xml_string_deserialize(char *xml_string, TREE_ITEM *root_tree);

/**
 * NAME					: xml_tree_find_element
 * DESCRIPTION	: find an element in a tree
 * INPUT	
 * 	root_tree		: container
 * 	path_to_find: the path in format elem/elem/elem example SS7AP/SCCP/CDPA
 * RETURNS
 * 	pointer of element in tree
 */
TREE_ITEM* xml_tree_find_element(TREE_ITEM *root_tree, char *path_to_find);

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
PROPERTY *xml_tree_find_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name);

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
int xml_tree_set_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name, char *new_value);

/**
 * NAME						: xml_tree_delete_attribute
 * DESCRIPTION		: set an xml attribute value
  * INPUT
 * 	root_tree			: container
 * 	path					: path to element that contains this in format elem/elem/elem example SS7AP/TCAP/COMPONENT/DATA
 * 	attrib_name		: name of the attribute example value
 * RETURNS
 * 	1							: success
 * 	0							: element or attribute name does not exists
 */
int xml_tree_delete_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name);


/**
 * NAME					: xml_tree_get_attribute
 * DESCRIPTION	: find an attribute
 * INPUT
 * 	root_tree		: container
 * 	path				: path to element that contains this in format elem/elem/elem example SS7AP/TCAP/COMPONENT/DATA
 * 	attrib_name	: name of the attribute example value
 * RETURNS
 * 	pointer to attribute
 */
char *xml_tree_get_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name, char *output, size_t max_size);

/**
 * NAME						: xml_tree_serialize
 * DESCRIPTION		: serialize xml tree to string
 * INPUT
 * 	root_tree			: container
 * 	outbuf				: string result
 * 	outmax				: maximum size
 * 	currlen				: current string length
 */
int xml_tree_serialize(TREE_ITEM *root_tree, char *outbuf, int outmax, int curr_len);



#endif
