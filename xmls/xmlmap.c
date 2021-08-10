#include <stdlib.h> 
#include <getopt.h>
#include <string.h>
#include "yxml.h"
#include <unistd.h>
#include "parserclass.h"
#include "xmlmap.h"

void split_elem_attrib(char *path, char *out_elem, char* out_attrib) {
	int last_delim = 0;
	int i = 0;
	int j = 0;
	
	while(path[i]){
		if (path[i] == '/'){
			last_delim = i;
		}
		out_elem[j] = path[i];
		i++;
		j++;
	}
	if (last_delim == 0){
		i = last_delim;
	}
	else {
		out_elem[last_delim ] = 0;
		i = last_delim + 1;
	}
	j = 0;
	while(out_elem[i]){
		out_attrib[j] = out_elem[i];
		out_elem[i] = 0;
		i++;
		j++;
	}

}



map_struct *new_map_struct() {
	return calloc(1, sizeof(map_struct));
}

map_struct *map_set_name(map_struct *in, char *input) {
	if (copy_string(&in->name, input)){
		return in;
	}
	return NULL;
}

map_struct *map_set_value(map_struct *in, char *input) {
	if (copy_string(&in->value, input)){
		return in;
	}
	return NULL;
}

int map_free(map_struct *in) {
	if (!in)
		return 1;
	if (in->name) {
		free(in->name);
		in->name = NULL;
	}
	if (in->value) {
		free(in->value);
		in->value = NULL;
	}
	free(in);
	return 1;
}

void map_print(map_struct *in, int ident) {
	for(int i = 0; i < ident; i++){
		fprintf(stdout, "\t");
	}
	if(!in) {
		fprintf(stdout, "NULL \n");
		return;
	}
	else if(!in->name){
		fprintf(stdout, "name: NULL");
	}
	else {
		fprintf(stdout, "name: %s", in->name);
	}
	if(!in->value) {
		fprintf(stdout, "\tvalue: NULL");
	}
	else {
		fprintf(stdout, "\tvalue: %s", in->value);
	}
	fprintf(stdout, "\n");
}

void priml_map_item_print(PPRIML_ITEM item, int ident) {
	if (item && item->data) {
		map_print((map_struct *) item->data, ident);
	}
}

void primlist_map_print(PPRIMLIST list, int ident) {
	PPRIML_ITEM currchild = NULL;
	char *data= list->get_data(list);
	if(data) {
		for(int i = 0 ; i < ident; i++) {
			fprintf(stdout, "\t");
		}
		fprintf(stdout, "%s\n", data);
	}
	currchild =  list->get_first_child(list);
	while(currchild) {
		priml_map_item_print(currchild, ident + 1);
		currchild = list->get_next_child(list);
	}
}

void primtreeitem_map_print(PPRIMTREE_ITEM item, int ident) {
	if (item) {
		primlist_map_print(&item->list, ident);
		PPRIMTREE_ITEM currchild = NULL;
		currchild = item->get_first_child(item);
		while(currchild) {
			primtreeitem_map_print(currchild, ident + 1);
			currchild = item->get_next_child(item);
		}
	}
}

int fn_map_value_cmp_by_name(map_struct *in, map_struct *out) {
	if (!out) {
		return 0;
	}
	return strcmp(in->name, out->name);
}	

int fn_map_value_cmp_by_name_no_sort(map_struct *in, map_struct *out) {
	if (!out) {
		return 0;
	}
	if (!strcmp(in->name, out->name)){
		return 0;
	}
	else {
		return 1;
	}
}	
int fn_map_value_cmp_by_value(map_struct *in1, map_struct *in2) {
	if (in1 && in2) {
		return strcmp(in1->value, in2->value);
	}
	return 0;
}

int fn_map_value_cmp_by_value_no_sort(map_struct *in1, map_struct *in2) {
	if (in1 && in2) {
		if(strcmp(in1->value, in2->value)) {
			return 1;
		}
	}
	return 0;
}
int strcmp_no_sort(char *s1, char *s2) {
	if(strcmp(s1, s2)) {
		return 1;
	}
	else {
		return 0;
	}
}
int xmls_map_unmarshall(char *xml_string, PRIMTREE_ITEM *root_tree) {
	char *doc_p = xml_string;
	PRIMTREE_ITEM *tree_active= root_tree;
	PRIMTREE_ITEM *tree_current;
	PPRIML_ITEM new_item;
	
	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	char tmp[1024] = {0};
	map_struct  *map_active = NULL;
	char *elem_name = NULL; 
	
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				elem_name = NULL;
				tree_current = newprimtreeitem();
				copy_string(&elem_name, xml_elem.elem);
				tree_current->set_data(tree_current, elem_name);
				tree_active->add_one(tree_active, tree_current, (int (*) (void *, void *))strcmp_no_sort);
				tree_active = tree_current;
				map_active = NULL;
				break;

			case YXML_ELEMEND:
				elem_name = NULL;
				//primtreeitem_print(tree_active, 0);
				tree_active = tree_active->parent;
				map_active = NULL;
				break;
				
			case YXML_ATTRSTART:
				map_active = new_map_struct();
				map_active = map_set_name(map_active, xml_elem.attr);
				break;
				
			case YXML_ATTRVAL:
				strcat(tmp, xml_elem.data);
				break;
				
			case YXML_ATTREND:
				map_set_value(map_active, tmp);
				memset(tmp, 0, sizeof(tmp));
				new_item = newpriml_item();
				new_item->set_data_remove_fn(new_item, (int (*) (void *))map_free);
				new_item->set_data(new_item, map_active);
				tree_active->list.add_one(&tree_active->list, new_item, (int (*) (void *, void *))fn_map_value_cmp_by_name_no_sort);
				map_active = NULL;
				break;
				
			default: 
				
				break;
		}
		doc_p++;
	}
	return 0;
}

int  xmlmap_marshall(PPRIMTREE_ITEM node, char *output, int outmax, int curr_len) {
	int i = curr_len;
	int j = 0;
	char *data;
	PPRIMTREE_ITEM  curr = NULL;
	map_struct *ms;
	if (node) {
		if ((data = node->get_data(node))) {
			if (i + 1 < outmax) {
				output[i++] = '<';
				while ((i + 1 < outmax) && data[j]){
					output[i++] = data[j++];
				}
				if (i + 1 < outmax) {
					output[i++] = ' ';
					PPRIML_ITEM item = node->list.get_first_child(&node->list);
					while (item) {
						if ((ms = (map_struct *)item->get_data(item))) {
							j = 0;
							while (i + 1 < outmax && ms->name[j]){
								output[i++] = ms->name[j++];
							}
							if (i + 4 < outmax ) {
								output[i++] = '=';
							}
							else {
								return i;
							}
							
							if (i + 3 < outmax ) {
								output[i++] = '"';
							}
							else {
								return i;
							}
							
							j = 0;
							while (i + 2 < outmax && ms->value[j]){
								output[i++] = ms->value[j++];
							}
							
							if (i + 2 < outmax) {
								output[i++] = '"';
							}
							if (i + 1 < outmax ) {
								output[i++] = ' ';
							}
							else {
								return i;
							}
						}
						item = node->list.get_next_child(&node->list);
					}
					if (i + 1 < outmax ) {
						output[i++] = '>';
					}
					else {
						return i;
					}
				}
			}
			else {
				return i;
			}
		}
		curr = node->get_first_child(node);
		while(curr) {
			i = xmlmap_marshall(curr, output, outmax, i);
			curr = node->get_next_child(node);
		}
		if ((data = node->get_data(node))) {
			if (i + 3 < outmax ) {
				j =  0;
				output[i++] = '<';
				output[i++] = '/';
				while((i + 1 < outmax) && data[j]) {
					output[i++] = data[j++];
				}
				output[i++] = '>';
			}
			else {
				return i;
			}
		}
	}
	return i;
}

PPRIMTREE_ITEM xmlmap_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

		while(*p_path == ' '){
			p_path ++;
		}

		start_path = p_path;
		while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
			p_path ++;
		}

		/* empty path or end of string */
		if (start_path == p_path){
			return node;
		}

		end_path = p_path;

		while (*p_path == ' ')
			p_path ++;

		if (*p_path == '/')
			p_path ++;

		strncpy(elem_name, start_path, end_path - start_path);
		
		if (! (next_tree = node->find_one(node, elem_name, (int (*) (void *, void *))strcmp_no_sort))) {
			return next_tree;
		}
		else {
			return xmlmap_find_element(next_tree, p_path);
		}
}
/*first implement tree add sorted*/
PPRIMTREE_ITEM xmlmap_add_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	char *new_string = NULL;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

	while(*p_path == ' '){
		p_path ++;
	}

	start_path = p_path;
	while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
		p_path ++;
	}

	/* empty path or end of string */
	if (start_path == p_path){
		return node;
	}

	end_path = p_path;

	while (*p_path == ' ')
		p_path ++;

	if (*p_path == '/')
		p_path ++;

	strncpy(elem_name, start_path, end_path - start_path);
	if (! (next_tree = node->find_one(node, elem_name, (int (*) (void *, void *))strcmp_no_sort))) {
		new_string = copy_string(&new_string, elem_name);
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, new_string);
		node->add_one(node, next_tree, (int (*) (void *, void *))strcmp_no_sort);
	}
	return xmlmap_add_element(next_tree, p_path);
	
}

map_struct* xmlmap_add_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute, char *value) {
	PPRIMTREE_ITEM found = NULL;
	if ((found = xmlmap_find_element(node, path_to_find)) == NULL) {
		found = xmlmap_add_element(node, path_to_find);
	}
	PPRIML_ITEM new_item = newpriml_item();
	map_struct *active_map = new_map_struct();
	map_set_name(active_map, attribute);
	map_set_value(active_map, value);
	new_item->set_data_remove_fn(new_item, (int (*) (void *))map_free);
	new_item->set_data(new_item, active_map);
	node->list.add_one(&node->list, new_item, (int (*) (void *, void *))fn_map_value_cmp_by_name_no_sort);
	return active_map;
}

PPRIML_ITEM xmlmap_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute) {
	PPRIMTREE_ITEM found = NULL;
	if ((found = xmlmap_find_element(node, path_to_find)) == NULL) {
		return NULL;
	}
	else {
		map_struct *active_map = new_map_struct();
		map_set_name(active_map, attribute);
		
		PPRIML_ITEM fattr = found->list.find_one(&found->list, active_map, (int (*) (void *, void *)) fn_map_value_cmp_by_name_no_sort);
		map_free(active_map);
		return fattr;

	}
}

map_struct* xmlmap_get_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute) {
	PPRIML_ITEM fattr = NULL;
	if ((fattr  = xmlmap_find_attribute(node, path_to_find, attribute))) {
		return fattr->get_data(fattr);
	}
	else {
		return NULL;
	}
}

int negate_strcmp(char *a, char *b) {
	int ret  = strcmp(a, b);
	return -1 *ret;
}
