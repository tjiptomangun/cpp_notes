CC = gcc
CFLAGS = 
LDFLAGS= -lpthread
T1 = unsynchronizedTest
T2 = synchronizedTest

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(T1) $(T2)

$(T1): unsynchronizedTest.o
	$(CC) -o $@ $< $(LDFLAGS)

$(T2): synchronizedTest.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm  -f  $(T1) $(T2) $(T1).o $(T2).o

	
