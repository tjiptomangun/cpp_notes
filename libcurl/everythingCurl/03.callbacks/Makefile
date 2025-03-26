CC=gcc -Wall -ggdb3
LINK=-lcurl
  
writeData: writeData.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 
clean:
	rm -rf writeData
