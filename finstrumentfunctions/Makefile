all: finsttest trace
	gcc -g -O0 -rdynamic --coverage -Winline  finsttest.o trace.o -ldl -o finsttest

finsttest: finsttest.c
	gcc -g  -Wall -c -finstrument-functions finsttest.c -o finsttest.o
trace: trace.c
	gcc -g   -Wall -c trace.c -o trace.o -D__USE_GNU -D_GNU_SOURCE

coverage: finsttest.c
	gcc -O0 --coverage finsttest.c -o finstcoverage

##for coverrage  make coverage && finstcoverage &&	gcov -i finsttest.c
