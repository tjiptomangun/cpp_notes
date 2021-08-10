#ifndef _XMLMAP_H_
#define _XMLMAP_H_
typedef struct {
	char *name;
	char *value;
} map_struct;

map_struct *new_map_struct();
map_struct *map_set_name(map_struct *in, char *input);
map_struct *map_set_value(map_struct *in, char *input);
int map_free(map_struct *in);
int xmls_map_unmarshall(char *xml_string, PRIMTREE_ITEM *root_tree);
int xmls_map_unmarshal_multiroot(char *xml_string, PRIMTREE_ITEM *root_tree);
int  xmlmap_marshall(PPRIMTREE_ITEM node, char *output, int outmax, int curr_len);
PPRIMTREE_ITEM xmlmap_find_element(PPRIMTREE_ITEM node, char *path_to_find);
PPRIMTREE_ITEM xmlmap_add_element(PPRIMTREE_ITEM node, char *path_to_find);
map_struct* xmlmap_add_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute, char *value);
PPRIML_ITEM xmlmap_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute);
map_struct* xmlmap_get_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute);
char * xmlmap_get_attribute_string(PPRIMTREE_ITEM node, char *path_to_find, char *attribute, char *out, int max_out);
#define ELEM_MAP_ITER_MAX 80
typedef struct xml_map_elem_iterator {
	int num;
	PPRIMTREE_ITEM ptrs[ELEM_MAP_ITER_MAX];
} XML_MAP_ELEM_ITERATOR;
XML_MAP_ELEM_ITERATOR *to_xml_map_iterator(PPRIMTREE_ITEM root, char *path, XML_MAP_ELEM_ITERATOR *iter);
#endif
