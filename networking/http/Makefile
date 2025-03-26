CC=gcc -Wall -ggdb3
LINK=-L/usr/lib/x86_64-linux-gnu 

all: readresponse01 
  
readresponse01: readresponse01.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

clean:
	rm -f readresponse01 
