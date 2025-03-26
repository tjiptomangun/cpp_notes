CC=gcc -Wall -ggdb3
LINK=-lcurl

all: follow01 follow02
  
follow01: follow01.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 
follow02: follow02.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

