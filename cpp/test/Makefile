all: testNamespace testNoNamespace

testNamespace: test.cpp
	g++ test.cpp -o test
testNoNamespace: testNoNamespace.cpp
	g++ testNoNamespace.cpp -o testNoNamespace

clean: 
	rm -rf testNamespace testNoNamespace test
