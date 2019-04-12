all: reverse  ranges

ranges: ranges.o 
	gcc -o target/ranges target/ranges.o

ranges.o: src/bin/ranges.c
	gcc -c src/bin/ranges.c -o target/ranges.o

reverse: reverse.o
	gcc -o target/reverse target/reverse.o

reverse.o: src/bin/reverse.c
	gcc -c src/bin/reverse.c -o target/reverse.o

clean:
	rm target/reverse target/reverse.o
	rm target/ranges target/ranges.o

.PHONY: all clean \
	reverse reverse.o \
	reverse reverse.o \

