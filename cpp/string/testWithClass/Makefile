CC=g++ -Wall -ggdb3

testAllocateAndReturn: testAllocateAndReturn.cpp
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 
	
