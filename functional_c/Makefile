all: test_list

compile: any.c option.c main.c
	gcc -Wall -ggdb3 -I . any.c option.c main.c -o functional_test
test_list: list.c
	gcc -Wall -ggdb3 -I . -D_LIST_UNIT_TEST_ list.c -o test_list

