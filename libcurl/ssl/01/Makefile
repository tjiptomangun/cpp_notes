CC=gcc -Wall -ggdb3
LINK=-lcurl

all: simplessl simplessl2
  
simplessl: simplessl.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

simplessl2: simplessl2.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

clean:
	rm -rf simplessl simplessl2

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

