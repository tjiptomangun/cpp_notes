all: reverse  ranges no_logic hextoint

ranges: ranges.o 
	gcc -o target/ranges target/ranges.o

ranges.o: src/bin/ranges.c
	gcc -c src/bin/ranges.c -o target/ranges.o

reverse: reverse.o
	gcc -o target/reverse target/reverse.o

reverse.o: src/bin/reverse.c
	gcc -c src/bin/reverse.c -o target/reverse.o

no_logic: no_logic.o
	gcc -o target/no_logic target/no_logic.o

no_logic.o: src/bin/no_logic.c
	gcc -Wall -ggdb3 -c src/bin/no_logic.c -o target/no_logic.o

hextoint: hextoint.o
	gcc -o target/hextoint target/hextoint.o

hextoint.o: src/bin/hextoint.c
	gcc -Wall -ggdb3 -c src/bin/hextoint.c -o target/hextoint.o

clean:
	rm target/reverse target/reverse.o
	rm target/ranges target/ranges.o
	rm target/no_logic target/no_logic.o
	rm target/hextoint target/hextoint.o

.PHONY: all clean \
	reverse reverse.o \
	ranges ranges.o \
	no_logic no_logic.o \
	hextoint hextoint.o \

