TARGET=printpids
CC=/usr/bin/gcc-10
LDLIBS=-lpthread
T1=printpids
T2=exitstatus
T3=incorrectexitstructure
T4=exitcleanup
T5=mutexProtectedDataStrucutre
T6=mutexExample01
T7=mutexExample02
T8=threadDataRace01
T9=deadlockMutex
TA=mutexProtectedSimplified
TB=mutexTimeout
TC=mutDestroyAndLock
TD=rwlock01
TE=rwlock02

.c.o:
	$(CC) -ggdb3 -c -Wall  -fsanitize=thread -o $@ $<
all: $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7) $(T8) $(T9) $(TA) $(TB) $(TC) $(TD) $(TE)
$(T1): ../err_apue.o $(T1).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T2): ../err_apue.o $(T2).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T3): ../err_apue.o $(T3).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T4): ../err_apue.o $(T4).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T5): ../err_apue.o $(T5).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T6): ../err_apue.o $(T6).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T7): ../err_apue.o $(T7).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T8): ../err_apue.o $(T8).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(T9): ../err_apue.o $(T9).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TA): ../err_apue.o $(TA).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TB): ../err_apue.o $(TB).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TC): ../err_apue.o $(TC).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TD): ../err_apue.o $(TD).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TE): ../err_apue.o $(TE).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
clean: 
	rm -f *.o ../*.o $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7) $(T8) $(T9) $(TA) $(TB) $(TC) $(TD) $(TE)

.PHONY: clean
