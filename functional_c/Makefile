all: test_list test_charstr test_key_val test_tuple mylambda1 mylambda2 mylambda3 curry1 nested_func curry2 trampoline

test_list: flist.c charstr.c integer.c tuple.c main.c
	gcc -Wall -ggdb3 -I . -D_FLIST_UNIT_TEST_ any.c charstr.c integer.c option.c flist.c tuple.c main.c -o test_list
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
curry1: curry1.c
	gcc -Wall -ggdb3 -I . curry1.c -o curry1
#	gcc -Wall -ggdb3 -I . -gdwarf-2 -g3 curry1.c -o curry1
curry1_dc: curry1.c
	gcc -Wall -ggdb3 -I . -D_COMPILE_ONLY_ -gdwarf-2 -g3 curry1.c -o curry1
nested_func: nested_func.c
	gcc -Wall -ggdb3 -I . nested_func.c -o nested_func
curry2: curry2.c
	gcc -Wall -ggdb3 -I . -Wtrampolines curry2.c -o curry2
trampoline: trampoline.c
	gcc -Wall -ggdb3 -I . -Wtrampolines trampoline.c -o trampoline

clean:
	rm -f functional_test test_list test_charstr test_key_val test_tuple mylambda1 mylambda2 mylambda3 curry1 nested_func curry2 trampoline

