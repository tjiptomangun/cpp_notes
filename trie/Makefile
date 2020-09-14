FLAGS=-Wall -ggdb3 -I .
CC=gcc ${FLAGS}

all: internal test rdx_internal rdxtree_test

internal: radixtree.c
	gcc -Wall -ggdb3 -I . radixtree.c  -o radixtree_internal -D_RADIXTREE_INTERNAL_TEST_

test: radixtree.c parserclass.c radixtree_test.c
	gcc -Wall -ggdb3 -I . radixtree.c  parserclass.c radixtree_test.c -o radixtree_test

rdx_internal: rdxtree.c
	gcc -Wall -ggdb3 -I . rdxtree.c  -o rdxtree_internal -D_RDXTREE_INTERNAL_TEST_

rdxtree_test: rdxtree.c parserclass.c rdxtree_test.c
	gcc -Wall -ggdb3 -I . rdxtree.c  parserclass.c rdxtree_test.c -o rdxtree_test
