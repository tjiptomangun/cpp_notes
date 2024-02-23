CC=g++ -Wall -ggdb3
all: transform01 withFunctor01

transform01: transform01.cpp
	${CC} -fsanitize=leak $^ -pedantic -llsan -lpthread ${LINK} -o $@ 


withFunctor01: withFunctor01.cpp
	${CC} -fsanitize=leak $^ -pedantic -llsan -lpthread ${LINK} -o $@ 



clean:
	rm -f transform01 withFunctor01
	
