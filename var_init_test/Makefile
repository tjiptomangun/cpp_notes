all: no_memset.c with_memset.c first_only.c
	gcc -Wall -ggdb3 no_memset.c -o no_memset
	gcc -Wall -ggdb3 with_memset.c -o with_memset
	gcc -Wall -ggdb3 first_only.c -o first_only

clean:
	rm -f no_memset with_memset first_only
