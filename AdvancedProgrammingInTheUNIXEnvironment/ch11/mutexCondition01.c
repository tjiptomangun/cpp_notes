#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Lewis B., Berg D.J. - Multithreaded programming with pthreads-Prentice Hall (1998)
 * It works like this: A thread obtains a mutex (condition variables always have an associated mutex)
 * and tests the condition under the mutex's protection. 
 * No other thread should alter any aspect of the condition without holding the mutex. 
 * If the condition is true, your thread completes its task, releasing the mutex when appropriate. 
 * If the condition isn't true, the mutex is released for you, and your thread goes to sleep(the mutex is 
 * released by the pthread library because the thread will sleep) on this
 * condition variable. When some other thread changes some aspect of the condition
 * , it calls pthread_cond_signal (), waking up one sleeping thread. Your thread then reacquires the mutex 
 * (your thread woken up and get the mutex by the library),
 *  reevaluates the condition, and either succeeds or goes back to sleep, depending upon the outcome. 
 * You must reevaluate the condition! First, the other thread may not have tested the complete condition 
 * before sending the wakeup. 
 * Second, even if the  condition was true when the signal was sent, it could have changed
 * before your thread got to run. Third, condition variables allow for spurious wakeups. 
 * They are allowed to wakeup for no discernible reason whatsoever!
 * page 128
 */

/**
 * sem_wait can be interrupted by signal which causes it return with value !=0
 */

pthread_mutex_t requests_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t requests_producer= PTHREAD_COND_INITIALIZER;
pthread_cond_t requests_consumer= PTHREAD_COND_INITIALIZER;


typedef struct req_t{
	struct req_t * next;
	int length;
	int * data;
}request_t, *prequest_t;


void release_req(prequest_t p) {
	if (p) {
		if (p->data) {
			free(p->data);
		}
		free(p);

	}
}

request_t * requests = NULL;
unsigned int req_length = 0;

void add(request_t *request) {
	req_length += 1;
	request->next = requests;
	requests = request;
}

prequest_t remv() {
	prequest_t request = NULL;

	if (requests) {
		request = requests;
		requests = requests->next;
		request->next = NULL;
		req_length -= 1;
		return request;
	}
	else {
		return NULL;

	}
}

void SEM_WAIT(sem_t *sem) {
	while (sem_wait(sem) != 0) {
		;
	}
}

unsigned int seed = 20;
int randInt(unsigned int maxValue) {
	return rand_r(&seed) % maxValue;
}

prequest_t read_from_net() {
	int length = randInt(20);
	int *data = (int *)calloc(length, sizeof(int));
	printf("read_from_net length %d\n", length);
	for(int i = 0; i < length; i++) {
		data[i] = randInt(200);
	}
	prequest_t request = (prequest_t)calloc(1, sizeof(request_t));
	request->length = length;
	request->data= data;
	return request;
}

prequest_t get_request() {
	return read_from_net();
}
unsigned int fib(int nthx) {

	unsigned int innerfib(unsigned int nth, unsigned int vn_1, unsigned int vn) {
		if (nth == 0){
			return vn_1;
		}
		else if (nth == 1){
			return vn;
		}
		else {
			return innerfib(nth - 1, vn, vn_1 + vn);
		}
	}
	return innerfib(nthx, 0, 1);
}
void process_request(prequest_t request) {
	printf("---------------\n");
	for(int i = 0; i < request->length; i++) {
		fib(request->data[i]);
	}
	printf("----------------request processed----------------\n");
} 

void * producer(void *arg) {
	prequest_t request;
	pthread_t tid = pthread_self();
	while(1) {
		request = get_request();
		pthread_mutex_lock(&requests_lock);// lock to test condition and wait, pthread_cond_wait will cause unlock
		printf("Thread producer [%ld]: lock \n", tid);
		while(req_length >= 10) {//test variable req_length changed only when holding mutex, 
		// add() and remv only called when holding request_lock
			printf("request_producer length %d, wait\n", req_length);
			pthread_cond_wait(&requests_producer, &requests_lock);
		}
		add(request);
		pthread_mutex_unlock(&requests_lock);
		printf("Thread producer [%ld]: unlock \n", tid);
		pthread_cond_signal(&requests_consumer);
	}
}
void * consumer(void *arg) {
	pthread_t tid = pthread_self();
	while(1) {
		pthread_mutex_lock(&requests_lock);// lock to test condition and wait, pthread_cond_wait will cause unlock
		printf("Thread consumer [%ld]: lock \n", tid);
		while(req_length == 0) {
			pthread_cond_wait(&requests_consumer, &requests_lock);
		}
		prequest_t request = remv();
		pthread_mutex_unlock(&requests_lock);
		printf("Thread consumer [%ld]: unlock \n", tid);
		printf("one request removed  by produces, signal request_producer\n"); 
		pthread_cond_signal(&requests_producer);
		process_request(request);
		release_req(request);
	}
}

int main(int argc, char **argv) {
	pthread_t p1, p2, p3, p4;
	if (pthread_create(&p1, NULL, &producer, NULL)) {
		return 1;
	}
	else if (pthread_create(&p2, NULL, &consumer, NULL)) {
		return 2;
	}
	else if (pthread_create(&p3, NULL, &consumer, NULL)) {
		return 3;
	}
	else if (pthread_create(&p4, NULL, &consumer, NULL)) {
		return 4;
	}

	if (pthread_join(p1, NULL)!= 0){
		return 5;
	}
	if (pthread_join(p2, NULL)!= 0){
		return 6;
	}
	if (pthread_join(p3, NULL)!= 0){
		return 7;
	}
	if (pthread_join(p4, NULL)!= 0){
		return 8;
	}
}

