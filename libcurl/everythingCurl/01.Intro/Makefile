CC=gcc -Wall -ggdb3
LINK=-lcurl
  
example: example.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 


clean: 
	rm -f example
