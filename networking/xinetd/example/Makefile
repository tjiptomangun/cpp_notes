C = gcc
CFLAGS = -O3 -g -Wall -Wextra -pedantic

default: server

server: budgerigar.o
	$(C) budgerigar.o -o budgerigar

server.o: budgerigar.c

clean:
	rm -f budgerigar *.o
