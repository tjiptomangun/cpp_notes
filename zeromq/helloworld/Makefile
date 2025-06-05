CC=gcc -Wall -ggdb3 -lzmq
LIB+=-lzmq
all: server client serverpair
server: server.c
	$(CC)  $^  $(LIB) -o $@

client: client.c
	$(CC)  $^  $(LIB) -o $@

serverpair: serverpair.c
	$(CC)  $^  $(LIB) -o $@
