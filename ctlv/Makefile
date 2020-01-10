ctlvtest: ctlv.c parserclass.c ctlvtest.c ctlvgen.c
	gcc -ggdb3 -Wall -fPIC -g -c ctlv.c
	gcc -shared -o libctlv.so.1.0 ctlv.o
	ln -s libctlv.so.1.0 libctlv.so
	gcc parserclass.c ctlvtest.c -Wall -ggdb3 -L. -lctlv -o ctlvtest 
	gcc ctlvgen.c -Wall -ggdb3 -o ctlvgen

clean:
	rm -f libctlv.so* ctlv.o ctlvtest ctlvgen
