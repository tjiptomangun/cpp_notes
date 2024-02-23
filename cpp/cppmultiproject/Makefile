LDFLAGS=-g
LDLIBS=-lterong
LDDIR=-L src/lib
INCDIR=-I src/inc

all: terong cabe

terong: one.o terongcreator.o target/libterong.a target/libterong.so
	g++ -o target/terongcreator one.o main.o target/libterong.a

target/libterong.a: src/lib/terong.cpp
	g++ -c $(INCDIR) src/lib/terong.cpp -o terong.o
	ar rvs target/libterong.a terong.o

target/libterong.so: src/lib/terong.cpp
	g++ -fPIC $(INCDIR) -c src/lib/terong.cpp -o terong.o
	g++ -shared -o target/libterong.so terong.o

one.o : src/bin/terongcreator/one.cpp
	g++ $(INCDIR) $(LDDIR) $(LDLIBS) -c src/bin/terongcreator/one.cpp -o one.o

terongcreator.o : src/bin/terongcreator/main.cpp
	g++ -c src/bin/terongcreator/main.cpp -o main.o

cabe: three.o cabecreator.o target/libcabe.a target/libcabe.so
	g++ -o target/cabecreator three.o main.o target/libcabe.a

target/libcabe.so: src/lib/cabe.cpp
	g++ -fPIC $(INCDIR) -c src/lib/cabe.cpp -o cabe.o
	g++ -shared -o target/libcabe.so cabe.o

target/libcabe.a: src/lib/cabe.cpp
	g++ -c $(INCDIR) src/lib/cabe.cpp -o cabe.o
	ar rvs target/libcabe.a cabe.o

three.o : src/bin/cabecreator/three.cpp
	g++ $(INCDIR) $(LDDIR) $(LDLIBS) -c src/bin/cabecreator/three.cpp -o three.o

cabecreator.o : src/bin/cabecreator/main.cpp
	g++ -c src/bin/cabecreator/main.cpp -o main.o

clean:
	rm main.o
	rm terong.o
	rm one.o
	rm target/terongcreator
	rm target/libterong.a
	rm target/libterong.so
	rm three.o
	rm cabe.o
	rm target/cabecreator
	rm target/libcabe.a
	rm target/libcabe.so


