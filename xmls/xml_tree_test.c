#include <stdio.h>
#include <stdlib.h> 
#include <getopt.h>
#include <string.h>
#include "parserclass.h"

int xml_string_deserialize(char *xml_string, TREE_ITEM *root_tree);
TREE_ITEM* xml_tree_find_element(TREE_ITEM *root_tree, char *path_to_find);

int main (int argc, char **argv) {
	int c;
	char buff[2048] = {0};
	TREE_ITEM *root_tree = newtreeitem(NULL, "root");
	TREE_ITEM *active_tree = root_tree;
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"stream", required_argument, 0, 's'},
			{"print_tree", no_argument, 0, 'p'},
			{"find_elem", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

		c  = getopt_long(argc, argv, "s:pf:", long_options, &option_index);

		if (c == -1){
			break;
		}

		switch (c) {	
			case 's' :
				strcpy(buff, optarg);
				xml_string_deserialize(buff, active_tree);
				break;
			case 'p' :
				if(active_tree)
					active_tree->list.l_item.class.printattributes(active_tree, 1);
				else
					fprintf(stdout, "empty tree");
				break;
			case 'f' :
				strcpy(buff, optarg);
				active_tree = xml_tree_find_element(active_tree, buff);
				break;
		}
		
	}

	root_tree->delete(root_tree);
	return 0;
}
