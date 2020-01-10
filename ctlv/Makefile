ctlvtest: ctlv.c parserclass.c ctlvtest.c ctlvgen.c
	gcc -m32 -ggdb3 -Wall -fPIC -g -c ctlv.c
	gcc -m32 -shared -o libctlv.so.1.0 ctlv.o
	ln -snf libctlv.so.1.0 libctlv.so
	gcc -m32 parserclass.c ctlvtest.c -Wall -ggdb3 -L. -lctlv -o ctlvtest 
	gcc ctlvgen.c -Wall -ggdb3 -o ctlvgen

clean:
	rm -f libctlv.so* ctlv.o ctlvtest ctlvgen
