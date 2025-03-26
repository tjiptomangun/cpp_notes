CC=g++

CFLAGS=-c -fPIC -m64 -std=c++14

OBJECTS=library.o

%.o:%.cpp
	$(CC) -c $(CFLAGS) $<

all: $(OBJECTS) main.o
	$(CC) -shared $(OBJECTS) -o liblibrary.so
	$(CC) main.o -L ./ -l library -o main


main.o: main.cpp
	 $(CC) -c main.cpp -o main.o -m64 

clean:
	rm rf $(OBJECTS) liblibrary.so main.o main


