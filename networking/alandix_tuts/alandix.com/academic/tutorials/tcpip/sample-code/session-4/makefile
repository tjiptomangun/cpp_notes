SDIR=tcp
CFLAGS=$(CCFLAGS) -I$(SDIR)/include 
#CC=acc -DANSI
CC=cc -DANSI
LINK=cc
INF=$(SDIR)/lib/inform.o
MYLIBS=$(SDIR)/lib/sock.a
LIBS=

conf:	server client

server:	server.o $(MYLIBS)
	$(LINK) $(CFLAGS) -o server server.o $(INF) $(MYLIBS)
client:	client.o $(MYLIBS)
	$(LINK) $(CFLAGS) -o client client.o $(INF) $(MYLIBS)

