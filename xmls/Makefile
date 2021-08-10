all: yxml.c xml_test.c xmls.c xml_tree.c parserclass.c xml_tree_test.c trie.c onepassxml.c xmlf_test.c xmlf.c
	gcc -Wall -ggdb3 -I. yxml.c xmls.c xml_test.c -o xml_test    
	gcc -Wall -ggdb3 -I. yxml.c xmls.c parserclass.c xml_tree.c xml_tree_test.c -o xml_tree_test
	gcc -Wall -ggdb3 -I. yxml.c xmls.c parserclass.c xml_tree.c xml_tree_unit_test.c -o xml_tree_unit_test
	gcc -Wall -ggdb3 -I. -DTRIE_TEST parserclass.c trie.c -o trie_test
	gcc -Wall -ggdb3 -I. -D_PARSER_CLASS_TEST_ yxml.c parserclass.c -o parserclass_test
	gcc -Wall -ggdb3 -I. yxml.c parserclass.c radixtree.c onepassxml.c -o onepassxml
	gcc -Wall -ggdb3 -I. yxml.c parserclass.c radixtree.c xmlf.c xmlf_test.c -o xmlf_test
	gcc -Wall -ggdb3 -I. yxml.c parserclass.c  xmle01.c xmle01_test.c -o xmle01_test
clean:
	rm -f xml_test xml_tree_test trie_test
