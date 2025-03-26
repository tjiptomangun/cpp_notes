#
# Warning: you may need more libraries than are included here on the
# build line.  The agent frequently needs various libraries in order
# to compile pieces of it, but is OS dependent and we can't list all
# the combinations here.  Instead, look at the libraries that were
# used when linking the snmpd master agent and copy those to this
# file.
#

CC=gcc

OBJS1=snmpdemoapp.o
OBJS2=example-demon.o nstAgentSubagentObject.o
OBJS3=asyncapp.o
TARGETS=example-demon snmpdemoapp asyncapp

CFLAGS=-I. `net-snmp-config --cflags`
BUILDLIBS=`net-snmp-config --libs`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared

nstAgentModules.so: nstAgentModules.c Makefile
	$(CC) $(CFLAGS) $(DLFLAGS) -c -o nstAgentModules.o nstAgentModules.c
	$(CC) $(CFLAGS) $(DLFLAGS) -o nstAgentModules.so nstAgentModules.o
