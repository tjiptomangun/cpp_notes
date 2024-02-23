#include "../apue.h"
#include "../miscfunc.h"
#include "attributeutils.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
struct to_info {
	void (*to_fn)(void *); /*function*/
	void *to_arg;
	struct timespec to_wait;
};

#define SECTONSEC 1000000000 /*seconds to nano seconds*/

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000		/* microseconds to nanoseconds */

void clock_gettime(int id, struct timespec *tsp) {
	struct timeval tv;

	gettimeofday(&tv, NULL);
	tsp -> tv_sec = tv.tv_sec;
	tsp -> tv_nsec = tv.tv_usec * USECTONSEC;
}

#endif

void *timeout_helper(void * arg) {
	struct to_info *tip;
	tip = (struct to_info *) arg;
	clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
	(*tip->to_fn)(tip->to_arg);
	free(arg);
	return (0);
}

void timeout (const struct timespec *when, void (*func)(void *), void *arg) {
	struct timespec now;
	struct to_info *tip;
	int		err;

	clock_gettime(CLOCK_REALTIME, &now);
	if((when->tv_sec > now.tv_sec) || (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
		tip = malloc(sizeof(struct to_info));
		if (tip != NULL) {
			tip->to_fn = func;
			tip->to_arg = arg;
			tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
			if (when->tv_nsec  >= now.tv_nsec) {
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
			} else {
				//means the when seconds > now seconds, but when nsec < now nsec. We need to reduce 
				//previous tip tv_sec result and covert it to nanosec as borrower to nsec calculation
				tip->to_wait.tv_sec --;
				tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec - when ->tv_nsec;
			}

			err = makethread(timeout_helper, (void *)tip, 1);
			if (err == 0) {
				return;
			}
			else {//if fail create thread just run the function, 
				  // this is why we need reentrant/recursive mutex
				 // because our function will lock the same mutex
				// from the same thread.
				free(tip);
				(*func)(arg);
			}
		}
	}
	else {
		//if when < now, just run the function, this is another reason
		//why reentrant/recursive mutex required
		(*func)(arg);
	}
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int finres = 0;
void theFunction() {
	finres = fib(3000);
	fprintf(stdout, "the fib is %d\n", finres);
}

void retry (void *arg) {
	pthread_mutex_lock(&mutex);
	theFunction();
	pthread_mutex_unlock(&mutex);
}

int main(void) {
	int err, condition, arg  = 0;
	struct timespec when;

	if((err = pthread_mutexattr_init(&attr))) {
		err_exit(err, "pthread_mutex_attr_init failed");
	} else if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE))) {
		err_exit(err, "pthread_mutex_attr_settype can't set recursive type");
	} else if ((err = pthread_mutex_init(&mutex, &attr))) {
		err_exit(err, "pthread_mutex_int can't create recursive mutex");
	}
	condition = finres == 0;

	pthread_mutex_lock(&mutex);
	/** check the condition under protection of a lock to make
	 * the check and call to timeout atomic
	 */
	if (condition) {

		clock_gettime(CLOCK_REALTIME, &when);
		when.tv_sec += 10;
		timeout(&when, retry, (void *)((unsigned long)arg));
	} 
	pthread_mutex_unlock(&mutex);
	sleep(12);

	exit(0);
		
}
