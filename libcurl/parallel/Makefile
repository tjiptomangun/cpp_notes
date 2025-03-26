CC=g++ -Wall -ggdb3
LINK=-lcurl

all: par01 notPar
  
par01: par01.cpp
	${CC} -fsanitize=leak  -fopenmp $^ -llsan ${LINK} -o $@ 

notPar: notPar.cpp
	${CC} -fsanitize=leak notPar.cpp -llsan ${LINK} -o $@ 

#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

