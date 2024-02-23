/**
 * Pthreads programming 156.8/550
 * simple condition variable
 *
 *
 * A call to pthread_cond_wait requires that a locked mutex be passed in 
 * along with the condition variable. 
 * The system releases the mutex on the caller’s behalf when the wait 
 * for the condition begins. 
 * In concert with the actions of the waiting thread, the thread that 
 * issues the pthread_cond_signal or pthread_cond_broadcast call holds 
 * the mutex at the time of the call but must release it after the call. 
 * Then, when the system wakes it up, a waiting thread can regain control 
 * of the mutex. 
 * It too must release the mutex when it’s finished with it.
 *
 * It all sounds complicated, but what if the mutex and the condition variable 
 * weren’t linked? If the condition were signaled without a mutex, 
 * the signaling thread might signal the condition before the waiting 
 * thread begins waiting for it—in which case the waiting thread would 
 * never wake up. If the system did not release the lock when the waiting 
 * thread entered the wait, no other thread could get the mutex and change the 
 * value of count such that the condition is met. The condition would never 
 * be signaled, and the program would deadlock. 
 * If the waiting thread didn’t release the mutex, 
 * no other thread could get the mutex. Here, too, we’d wind up in a deadlock.
 */

#include <stdio.h>
#include <pthread.h>

#define TCOUNT 20
#define WATCH_COUNT 12


int count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_threshold_cv = PTHREAD_COND_INITIALIZER;
int thread_ids[3] = {0, 1, 2};

void * watch_count(void *idp);
void * inc_count(void *idp);
int main(void) {
	int i;
	pthread_t threads[3];

	pthread_create(&threads[0], NULL, inc_count, &thread_ids[0]);
	pthread_create(&threads[1], NULL, inc_count, &thread_ids[1]);
	pthread_create(&threads[2], NULL, watch_count, &thread_ids[2]);
	for(i = 0; i < 3; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}

void * watch_count(void *kidp) {
	int * idp = (int *) kidp;
	pthread_mutex_lock(&count_mutex);
	while(count <= WATCH_COUNT) {
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("watch_count(): Thread %d, count is %d\n", *idp, count);
	}
	pthread_mutex_unlock(&count_mutex);
	
}

void * inc_count(void *kidp) {
	int * idp = (int *) kidp;
	sleep(1);
	for(int i = 0; i < TCOUNT ; i++) {
		pthread_mutex_lock(&count_mutex);
		count ++;
		printf("inc_count(); Thread %d, old count %d, new count %d\n", *idp, count - 1, count);
		if (count == WATCH_COUNT) {
			pthread_cond_signal(&count_threshold_cv);
		} 
		pthread_mutex_unlock(&count_mutex);
		
	}
	printf("inc_count(); Thread %d, exit %d\n", *idp);
	
}
