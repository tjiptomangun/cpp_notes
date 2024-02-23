CC=/usr/bin/gcc-10
LDLIBS=-lpthread
T1=recursive01

.c.o:
	$(CC) -ggdb3 -c -Wall  -fsanitize=thread -o $@ $<
all: $(T1) 
$(T1): ../err_apue.o $(T1).o attributeutils.o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
clean: 
	rm -f *.o ../*.o $(T1) 

.PHONY: clean
