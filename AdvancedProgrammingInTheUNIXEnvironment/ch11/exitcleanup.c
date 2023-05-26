/**
 * clean up handler are calls when
 * . makes a call to pthread_exit
 * . responds to cancellation request
 * . makes a call to pthread_cleanup_pop with a nonzero argument
 */
#include "../apue.h"
#include <pthread.h>
void cleanup(void *arg) {
	printf("cleanup: %s\n", (char *) arg);
}
void* thr_fn1(void *arg) {
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	if (arg) {
		return ((void *)1);
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return ((void *)1);
}

void* thr_fn2(void *arg) {
	printf("thread 2 start\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");


	if (arg) {//will go here
		pthread_exit((void *)2);
	}
	pthread_cleanup_pop(0);//this two mandatory because
	pthread_cleanup_pop(0);// pthread_cleanup_push() most likely is implemented 
						   // as a macro introducing an open brace { which expects a 
						   // (corresponding) pthread_cleanup_pop() in the same context. 
						   // The latter then serves the closing brace }. 
						   // ref https://stackoverflow.com/a/20304575/8642222
	pthread_exit((void *)2);
}

int main() {
	int err;
	pthread_t tid1, tid2;
	void *tret;

	err = pthread_create(&tid1, NULL, thr_fn1, (void *) 1);
	if (err != 0) {
		err_quit("can't create thread 1: %s\n", strerror(err));
	}
	err = pthread_create(&tid2, NULL, thr_fn2, (void *) 1);
	if (err != 0) {
		err_quit("can't create thread 2: %s\n", strerror(err));
	}
	err = pthread_join(tid1, &tret);
	if (err != 0) {
		err_quit("can't join thread 1: %s\n", strerror(err));
	}
	printf("thread 1 exit code %d \n", (int)(long int) tret);
	sleep(1);
	err = pthread_join(tid2, &tret);
	if (err != 0) {
		err_quit("can't join thread 2: %s\n", strerror(err));
	}
	printf("thread 2 exit code %d \n", (int)(long int) tret);
	exit(0);
}
