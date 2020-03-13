FLAGS=-Wall -ggdb3 -I .
CC=gcc ${FLAGS}

all: internal test

internal: radixtree.c
	gcc -Wall -ggdb3 -I . radixtree.c  -o radixtree_internal -D_RADIXTREE_INTERNAL_TEST_

test: radixtree.c parserclass.c radixtree_test.c
	gcc -Wall -ggdb3 -I . radixtree.c  parserclass.c radixtree_test.c -o radixtree_test
