#include <stdio.h>
#include <stdlib.h> 
#include <getopt.h>
#include <string.h>
#include "parserclass.h"

int xml_string_deserialize(char *xml_string, TREE_ITEM *root_tree);
TREE_ITEM* xml_tree_find_element(TREE_ITEM *root_tree, char *path_to_find);
PROPERTY *xml_tree_find_attribute(TREE_ITEM *root_tree, char *path, char *attrib_name);
int xml_tree_serialize(TREE_ITEM *root_tree, char *outbuf, int outmax, int curr_len);

void usage(char *app) {
	fprintf (stdout, "usage : %s -s xml_string f:g:p\n\
					  %s -h\n\
					 -i --string input: input to parse xml\n\
					 -p : print tree\n\
					 -f find element input: to find and element\n\
					 -g --get input:to element of name and print \n\
					 -s --serialize : print current tree to xml format\n"
					 , app, app);
}


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
int main (int argc, char **argv) {
	int c;
	char buff[2048] = {0};
	char elem[2048] = {0};
	char attrib[2048] = {0};
	char val[256] = {0};
	TREE_ITEM *root_tree = newtreeitem(NULL, "root");
	TREE_ITEM *active_tree = root_tree;
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"i", required_argument, 0, 'i'},
			{"print", no_argument, 0, 'p'},
			{"find", required_argument, 0, 'f'},
			{"get", required_argument, 0, 'g'},
			{"help", no_argument, 0, 'h'},
			{"serialize", no_argument, 0, 's'},
			{0, 0, 0, 0}
		};

		c  = getopt_long(argc, argv, "i:pf:g:hs", long_options, &option_index);

		if (c == -1){
			break;
		}

		switch (c) {	
			case 'i' :
				strcpy(buff, optarg);
				xml_string_deserialize(buff, active_tree);
				break;
			case 'p' :
				if(active_tree)
					active_tree->list.l_item.class.printattributes((PCLASS)active_tree, 1);
				else
					fprintf(stdout, "empty tree");
				break;
			case 'f' :
				strcpy(buff, optarg);
				active_tree = xml_tree_find_element(active_tree, buff);
				break;
			case 'g' :
				strcpy(buff, optarg);
				split_elem_attrib(buff, elem, attrib);
				PROPERTY *prop = xml_tree_find_attribute(active_tree, elem, attrib);
				fprintf(stdout, "elem %s\n", elem);
				if (prop){
					prop->getvalue(prop, val);
					fprintf(stdout, "attr %s value = %s\n", attrib, val);
					prop->l_item.class.printattributes((PCLASS)prop, 0);
				}
				else
					fprintf(stdout, "attr %s value = [empty]\n", attrib);
				break;
			case 'h':
				usage(argv[0]);
				root_tree->delete(root_tree);
				return 0;
			case 's':
				memset(buff, 0, 2048);
				xml_tree_serialize(active_tree, buff, 2048, 0);
				fprintf(stdout, "%s\n", buff);
				break;
		}
		
	}

	root_tree->delete(root_tree);
	return 0;
}

