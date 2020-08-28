all: yxml.c xml_test.c xmls.c xml_tree.c parserclass.c xml_tree_test.c trie.c
	gcc -Wall -ggdb3 -I. yxml.c xmls.c xml_test.c -o xml_test    
	gcc -Wall -ggdb3 -I. yxml.c xmls.c parserclass.c xml_tree.c xml_tree_test.c -o xml_tree_test
	gcc -Wall -ggdb3 -I. -DTRIE_TEST parserclass.c trie.c -o trie_test
	gcc -Wall -ggdb3 -I. -D_PARSER_CLASS_TEST_ yxml.c parserclass.c -o parserclass_test
clean:
	rm -f xml_test xml_tree_test trie_test
