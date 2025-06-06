#include "../apue.h"
#include "pthread.h"

pthread_t ntid;

void printids(const char *s) {
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	unsigned int utid = (unsigned int) tid;
	unsigned int upid = (unsigned int) pid;
	printf("%s pid %u tid %u (0x%x) \n", s, upid, utid, utid);
}

void * thr_fn(void *arg) {
	printids("new thread: ");
	return (void *) 0;
}

int main (void) {
	int err;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0) {
		err_quit("can't create thread: %s\n", strerror(err));
	}
	printids("main thread: ");
	sleep(1);
	exit(0);
}
