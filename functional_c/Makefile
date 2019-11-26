all: test_list test_charstr test_key_val test_tuple mylambda1 mylambda2 mylambda3

test_list: list.c charstr.c integer.c tuple.c main.c
	gcc -Wall -ggdb3 -I . -D_LIST_UNIT_TEST_ any.c charstr.c integer.c option.c list.c tuple.c main.c -o test_list
test_tuple: tuple.c charstr.c main.c
	gcc -Wall -ggdb3 -I . -D_TUPLE_UNIT_TEST_ any.c charstr.c integer.c tuple.c main.c -o test_tuple
test_charstr: charstr.c
	gcc -Wall -ggdb3 -I . -D_CHARSTR_TEST_ any.c charstr.c -o test_charstr
test_key_val: charstr.c key_val.c any.c
	gcc -Wall -ggdb3 -I . -D_KEY_VAL_TEST_ any.c charstr.c key_val.c -o test_key_val
mylambda1: mylambda1.c
	gcc -Wall -ggdb3 -I . mylambda1.c -o mylambda1
mylambda2: mylambda2.c
	gcc -Wall -ggdb3 -I . mylambda2.c -o mylambda2
mylambda3: mylambda3.c
	gcc -Wall -ggdb3 -I . -gdwarf-2 -g3 mylambda3.c -o mylambda3

clean:
	rm -f functional_test test_list test_charstr test_key_val test_tuple mylambda1 mylambda2 mylambda3

