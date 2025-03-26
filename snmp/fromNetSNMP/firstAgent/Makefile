#
# Warning: you may need more libraries than are included here on the
# build line.  The agent frequently needs various libraries in order
# to compile pieces of it, but is OS dependent and we can't list all
# the combinations here.  Instead, look at the libraries that were
# used when linking the snmpd master agent and copy those to this
# file.
#
CC=gcc -ggdb3
OBJS1=snmpdemoapp.o
OBJS2=snmpdemotocentos8.o
OBJS3=snmpinformdemo.o
OBJS4=snmpsetdemo.o
TARGETS=snmpdemoapp snmpdemotocentos8 snmpinformdemo snmpsetdemo

CFLAGS=-I. `net-snmp-config --cflags`
BUILDLIBS=`net-snmp-config --libs`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared


all: $(TARGETS)
snmpdemoapp: $(OBJS1)
	$(CC) -o snmpdemoapp $(OBJS1) $(BUILDLIBS)
snmpdemotocentos8: $(OBJS2)
	$(CC) -o snmpdemotocentos8 $(OBJS2) $(BUILDLIBS)
snmpinformdemo: $(OBJS3)
	$(CC) -o snmpinformdemo $(OBJS3) $(BUILDLIBS)
snmpsetdemo: $(OBJS4)
	$(CC) -o snmpsetdemo $(OBJS4) $(BUILDLIBS)
clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(TARGETS)

