CC=g++
CFLAGS=-fPIC -m64
OBJECTS=logregx.o

%.o:%.c
	$(CC) -c $(CFLAGS) $<

all:$(OBJECTS)
	$(CC) -shared $(OBJECTS) -o liblogregx.so

clean:
	rm -f *.o liblogregx.so
