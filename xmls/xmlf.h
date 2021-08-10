#ifndef _XML_F_H_
#define _XML_F_H_
#include "parserclass.h"
/**
 * name 						:is name of this element
 * update_content		:function to update config_ptr. 
 *										1st parameter of this function will be configPtr,
 *										2nd  parameter of this function will be content of xml
 * extract_content 	:flag whether to store this xml content to configPtr
 * dst_ptr				:where to store the result, this is destination (1st param)
 * 									 of update_content
 */
typedef struct {
	char name [80];
	int (*update_content) (void*, char*);
	int extract_content; //if 1 mark the last path of element, 0 have next element
	void *dst_ptr; //destination to store the result
}xfn_element_data;


/**
 * name 						:is name of this attribute
 * update_value			:function to update config_ptr. 
 *										1st parameter of this function will be configPtr,
 *										2nd  parameter of this function will be content of xml
 * dst_ptr				:where to store the result, this is destination (1st param)
 * 									 of update_content
 */
typedef struct {
	char name [80];
	int (*update_value) (void*, char*);
	void *dst_ptr; //destination to store the result
}xfn_attribute_data;

xfn_attribute_data * new_xfn_attribute_data(char *name, int (*update_value) (void *, char*), void *dest);

xfn_element_data * new_xfn_element_data(char *name, int (*update_value) (void *, char*), int extract_content, void *dest);

int xfn_element_cmp(xfn_element_data * c1, xfn_element_data *c2);

int xfn_attribute_cmp(xfn_attribute_data * c1, xfn_attribute_data * c2);

int xfn_do_nothing(void *dest, char *in);


/**
 * NAME 				: xmlf_add_element
 * DESCRIPTION	: add one element relative to node with path path_to_find. 
 * 								If element already exists will not add or update
 */
PPRIMTREE_ITEM xmlf_add_element(PPRIMTREE_ITEM node, char *path_to_find, int (*update_value) (void *, char*), void *dest);

/**
 * xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: data that stored in this element
 */
void * xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find);

/**
 * NAME				 	: xmlf_find_attribute
 * DESCRIPTION 	: find attribute data attached to path_to_find attribute_name
 * 								this data is previously allocated
 */
xfn_attribute_data * xmlf_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name);

#endif
