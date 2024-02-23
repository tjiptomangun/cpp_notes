#include "../apue.h"
#include <pthread.h>

int makethread(void *(*fn)(void *), void *arg, int detached) {
	int err;
	pthread_t	tid;
	pthread_attr_t attr;
	if((err = pthread_attr_init(&attr))) {
		return (err);
	} 
	else if ((err = pthread_attr_setdetachstate(&attr, detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE))){
		return (err);
	}
	else if ((err = pthread_create(&tid, &attr, fn, arg))) {
		pthread_attr_destroy(&attr);
		return (err);
	} else if ((err = pthread_attr_destroy(&attr))) {
		exit(1)	;
	} {
		return 0;
	} 
}
