FLAGS=-Wall -ggdb3 -I .
CC=gcc ${FLAGS}

echosvr : echo-all.c sock.c 
	$(CC) echo-all.c sock.c  -o echosvr

proxysvr: proxy.c sock.c parserclass.c  parsecfg.c 
	$(CC) proxy.c sock.c parserclass.c  parsecfg.c -o proxysvr
simpleclient: simpleclient.c 
	$(CC) simpleclient.c -o simpleclient

all: echosvr proxysvr simpleclient

clean:
	rm echosvr proxysvr simpleclient
