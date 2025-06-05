CC=gcc -Wall -ggdb3
all: smpppdu verifyPDUTag

smpppdu: smpppdu.c tcp.c logger.c
	gcc -Wall -ggdb3 tcp.c smpppdu.c logger.c -o smpppdu -lpthread -lm
verifyPDUTag: verifyPDUTag.c
	${CC} $^ ${LINK} -o $@ 
