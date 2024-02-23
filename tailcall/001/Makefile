CC=gcc
CFLAGS= -g -O -o
CFLAGSS= -gdwarf-2 -O2 -o
CFLAGST= -ggdb3 -o
T1=test001
T2=infrecursive

all:$(T1) $(T1)s $(T1)t $(T2) $(T2)s $(T2)t

$(T1):$(T1).c
	$(CC) $(CFLAGS) $@ $^

$(T1)s:$(T1).c
	$(CC) $(CFLAGSS) $@ $^

$(T1)t:$(T1).c
	$(CC) $(CFLAGST) $@ $^

$(T2):$(T2).c
	$(CC) $(CFLAGS) $@ $^

$(T2)s:$(T2).c
	$(CC) $(CFLAGSS) $@ $^

$(T2)t:$(T2).c
	$(CC) $(CFLAGST) $@ $^

clean:
	rm -f $(T1) $(T1)s $(T1)t $(T2) $(T2)s $(T2)t
