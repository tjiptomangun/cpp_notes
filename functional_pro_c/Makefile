all: test
test: test.c lutils.h
	gcc -std=c99 -I/usr/include/glib-2.0/ -I /usr/lib64/glib-2.0/include -Wall -ggdb3 -lglib-2.0 test.c -o test

expand: test.c lutils.h
	gcc -E -std=c99 -I/usr/include/glib-2.0/ -I /usr/lib64/glib-2.0/include -Wall -ggdb3 test.c

clean:
	rm test
