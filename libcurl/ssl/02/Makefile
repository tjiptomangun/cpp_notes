CC=gcc -Wall -ggdb3
LINK=-lcurl

all: regoogle
  
regoogle: regoogle.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

clean:
	rm -rf regoogle

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

