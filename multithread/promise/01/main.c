#include <stdio.h>
#include "future.h"

void *func (void *arg) {
	promise *p = (promise *) arg;
	msg("started thread");
	sleep(3);
	msg("\ttthread will set promise");
	promise_set(p, 42);
	msg("\tstopping thread");
	return NULL;
}


int main(int argc, char **argv) {
	long t;
	msg("main thread");
	future *f = future_create(func);
	promise *p = promise_create();
	msg("promise created %p", p);
	future_start(f, p);
	msg("got result from future, %d", promise_get(p));
	future_close(f);
	promise_close(p);
	pthread_exit(NULL);
}
