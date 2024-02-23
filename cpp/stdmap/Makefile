CC=g++ -Wall -ggdb3

testMap: testMap.cpp trimStr.cpp
	${CC} -fsanitize=leak $^ -pedantic -llsan -lpthread ${LINK} -o $@ 

trimStr: trimStr.cpp
	${CC} -fsanitize=leak -DTEST $^ -pedantic -llsan -lpthread ${LINK} -o $@ 


clean:
	rm -f testMap.o testMap trimStr
	
