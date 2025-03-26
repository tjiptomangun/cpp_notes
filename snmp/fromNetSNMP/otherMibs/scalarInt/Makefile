
CC=gcc
TABLE_PREFIX=scalar_int

NETSNMPCONFIG=net-snmp-config


# uncomment this if you have GNU make
#NETSNMPCFLAGS := $(shell $(NETSNMPCONFIG) --base-cflags)
#NETSNMPLIBS := $(shell $(NETSNMPCONFIG) --agent-libs)
NETSNMPCFLAGS=`$(NETSNMPCONFIG) --base-cflags`
NETSNMPLIBS=`$(NETSNMPCONFIG) --agent-libs`

LIBS=$(NETSNMPLIBS) -fPIC


STRICT_FLAGS = -Wall -Wstrict-prototypes
CFLAGS=-I. $(NETSNMPCFLAGS) $(STRICT_FLAGS) -fPIC

USER_SRCS = \
	scalar_int.c \ 


SRCS = $(USER_SRCS) \

USER_OBJS =  \
	scalar_int.o\

OBJS =  $(USER_OBJS) \

TARGETS=$(TABLE_PREFIX)

.SUFFIXES:
.SUFFIXES: .c .o .deps


all: $(TARGETS)

user: $(USER_OBJS)

$(TARGETS): $(LIB_DEPS)

$(TABLE_PREFIX): $(OBJS) Makefile
	$(CC) -o $(TABLE_PREFIX) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGETS)

