CC=g++
CFLAGS=-fPIC -m64
OBJECTS=add.o answer.o

all:$(OBJECTS) main.o
	$(CC) -shared $(OBJECTS) -o bin/shared/libtq84.so
	$(CC) bin/main.o -Lbin/shared -ltq84 -o bin/dynamically-linked


%.o: src/tq84/%.c
	$(CC) -c $(CFLAGS) $<


main.o: src/main.c
	$(CC) -c src/main.c -o bin/main.o -m64

clean:
	rm -f $(OBJECTS) bin/shared/libtq84.so bin/main.o bin/dynamically-linked

