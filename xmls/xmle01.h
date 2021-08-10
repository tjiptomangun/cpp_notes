#ifndef _XML_E01_H_
#define _XML_E01_H_
#include "parserclass.h"
/**
 * name 						:is name of this element
 */
typedef struct {
	char name [80];
}xe01_element_data;


/**
 * name 						:is name of this attribute
 * update_value			:function to update config_ptr. 
 *										1st parameter of this function will be configPtr,
 *										2nd  parameter of this function will be content of xml
 * ptr_only					:whether this parse attribute store pointer only to data in 
 *                   xml string
 * ptr_passed				:true if ptr_only pointer has been set
 * 
 * 
 */
typedef struct {
	char name [80];
	int (*update_value) (void *, char*);
	int ptr_only;
	int ptr_passed;
}xe01_attribute_data;
xe01_attribute_data * new_xe01_attribute_data(char *name, int (*update_value) (void *, char*), int ptr_only);
xe01_attribute_data * xmle01_add_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name, xe01_attribute_data *data);
int xmle01_parsedata(char *xml_string, PRIMTREE_ITEM *fn_data, void * dest);
xe01_attribute_data * xmle01_unset_ptr_passed(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name);
#endif
