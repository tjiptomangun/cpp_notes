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
TF=rwlock03
TG=semaphore01
TH=mutexCondition01
TI=barrier01
TJ=barrier02
TK=barrier03
TL=mutexCondition02
TM=mutexCondition03

.c.o:
	$(CC) -DDEBUG -ggdb3 -c -Wall  -fsanitize=thread -o $@ $<
all: $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7) $(T8) $(T9) $(TA) $(TB) \
	$(TC) $(TD) $(TE) $(TF) $(TG) $(TH) $(TI) $(TJ) $(TL) $(TM)
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
$(TF): ../err_apue.o $(TF).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TG): ../err_apue.o $(TG).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TH): ../err_apue.o $(TH).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TI): ../err_apue.o $(TI).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TJ): ../err_apue.o $(TJ).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS) -lbsd
$(TK): ../err_apue.o $(TK).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
$(TL): ../err_apue.o $(TL).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS) 
$(TM): ../err_apue.o $(TM).o
	$(CC) -g -m64 -fsanitize=thread -o $@ $^ $(LDLIBS)
clean: 
	rm -f *.o ../*.o $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7) $(T8) $(T9) $(TA) $(TB) $(TC) $(TD) \
	$(TE) $(TF) $(TG) $(TH) $(TI) $(TJ)  $(TK) $(TL) $(TM)

.PHONY: clean
