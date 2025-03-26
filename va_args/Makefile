all: va_arg_test.c va_arg_format.c vaconcat.c
	gcc -Wall -ggdb3 va_arg_test.c -o va_arg_test
	gcc -Wall -ggdb3 va_arg_format.c -o va_arg_format
	gcc -Wall -ggdb3 va_printf.c -o va_printf
	gcc -Wall -ggdb3 vaconcat.c -o vaconcat

clean:
	rm va_arg_test va_arg_format va_printf vaconcat
