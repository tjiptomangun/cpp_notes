all: test_list test_charstr compile test_key_val

compile: any.c option.c main.c
	gcc -Wall -ggdb3 -I . any.c option.c main.c -o functional_test
test_list: list.c charstr.c integer.c
	gcc -Wall -ggdb3 -I . -D_LIST_UNIT_TEST_ any.c charstr.c integer.c list.c -o test_list
test_charstr: charstr.c
	gcc -Wall -ggdb3 -I . -D_CHARSTR_TEST_ any.c charstr.c -o test_charstr
test_key_val: charstr.c key_val.c any.c
	gcc -Wall -ggdb3 -I . -D_KEY_VAL_TEST_ any.c charstr.c key_val.c -o test_key_val

clean:
	rm -f functional_test test_list functional_test test_key_val

