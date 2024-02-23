CC=/usr/bin/gcc-10
LDLIBS=-lpthread
T1=lslike
T2=stdinstdout01

.c.o:
	$(CC) -ggdb3 -c -Wall  -o $@ $<
all: $(T1) $(T2)

$(T1): ../err_apue.o $(T1).o 
	$(CC) -g -m64 -o $@ $^ $(LDLIBS)

$(T2): ../err_apue.o $(T2).o 
	$(CC) -g -m64 -o $@ $^ $(LDLIBS)

clean: 
	rm -f *.o ../*.o $(T1) $(T2)

.PHONY: clean
