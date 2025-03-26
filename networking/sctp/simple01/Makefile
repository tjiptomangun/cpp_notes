CC=gcc -Wall -ggdb3
all: sctpserver sctpclient sctpclient2 sctpechoall sctpechoclient
sctpserver: sctpserver.c
	$(CC) sctpserver.c -lsctp -o sctpserver
sctpclient: sctpclient.c
	$(CC) sctpclient.c -lsctp -o sctpclient
sctpclient2: sctpclient2.c
	$(CC) sctpclient2.c -lsctp -o sctpclient2
sctpechoall: sctpechoall.c
	$(CC) sctpechoall.c sctpsock.c -lsctp -o sctpechoall
sctpechoclient: sctpechoclient.c
	$(CC) sctpechoclient.c sctpsock.c -lsctp -o sctpechoclient
clean:
	rm -f sctpserver sctpclient sctpclient2 sctpechoall sctpechoclient

