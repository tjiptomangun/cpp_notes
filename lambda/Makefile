
all: lambda1 lambdamultiline

lambda1 : lambda1_test.c
	gcc -Wall -ggdb3 lambda1_test.c -o lambda1_test 
lambdamultiline : lambdamultiline_test.c
	gcc -Wall -ggdb3 lambdamultiline_test.c -o lambdamultiline_test 

clean:
	rm -f lambda1_test lambdamultiline_test

