#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define handle_error_en(en, msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)

static void display_pthread_attr(pthread_attr_t *attr, char *prefix) {
	int s, i;
	size_t v;
	void *stkaddr;
	struct sched_param sp;

	s = pthread_attr_getdetachstate(attr, &i);
	if (s != 0){
		handle_error_en(s, "pthread_attr_getdetachstate");
	}
	printf("%sDetach state		= %s\n", prefix,
		(i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
		(i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
		"???"
		);

	s = pthread_attr_getscope(attr, &i);
	if (s != 0){
		handle_error_en(s, "pthread_attr_getscope");
	}
}


