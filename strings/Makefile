CC=gcc -Wall -ggdb3
all: strings strtok_test  stringsstack stringptrref reversestring
strings: strings.c
	$(CC) strings.c -o strings
strtok_test:
	$(CC) strtok_test.c -o strtok_test
stringsstack: stringsstack.c
	$(CC) stringsstack.c -o stringsstack
stringptrref: stringptrref.c
	$(CC) stringptrref.c -o stringptrref
reversestring: reversestring.c
	$(CC) $^ -o $@

