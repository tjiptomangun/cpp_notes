all: reverse 

reverse: reverse.o
	gcc -o target/reverse target/reverse.o

reverse.o: src/bin/reverse.c
	gcc -c src/bin/reverse.c -o target/reverse.o

clean:
	rm target/reverse target/reverse.o

.PHONY: all clean reverse reverse.o
