//ben-books-master/POSIX_THREAD/MultiThreaded-Programming-With-POSIX.pdf page 7
//thread-pool-server.c {page 28}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_HANDLER_THREADS 3


/**
 * global mutex for our program. assignment initializes it. 
 * Note that use RECURSIZE mutex, since a handler mutex might
 * try to lock it twice consecutively
 */
//pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t request_mutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutexattr_t attr;

pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;
int num_requests = 0; 

int done_creating_requests = 0;

/**
 * format of a single request
 */

struct request {
	int number;
	struct request * next;
};

struct request *requests = NULL; /*head of linked list of request*/
struct request *last_request = NULL; /*pointer to last request*/

/**
 * function add_request() : add a request to the requests list
 * algorithm : creates a request structure, adds to the list, 
 * and increases number of pending request by one
 * input: request number, linked list mutex
 * output: none
 *
 */
void add_request(int request_num, pthread_mutex_t *p_mutex, pthread_cond_t *p_cond_var) {
	int rc;

	struct request *a_request;

	/* create structure with new request*/
	a_request = (struct request *) malloc(sizeof(struct request));
	if (!a_request) {
		fprintf(stderr, "add request: out of memory\n");
		exit(1);
	}

	a_request->number = request_num;
	a_request->next = NULL;

	/* lock the mutex, to assure exclusive access to the list*/
	rc = pthread_mutex_lock(p_mutex);

	/* add new request to the end of the list, updating list pointers as 
     * required*/
	if (num_requests == 0) {
		requests = a_request;
		last_request = a_request;
	} else {
		last_request->next = a_request;
		last_request = a_request;
	}
	num_requests ++;

#ifdef DEBUG
	printf("add request: added request with id '%d'\n", a_request->number);
	fflush(stdout);
#endif //DEBUG

	/* unlock mutex */
	rc = pthread_mutex_unlock(p_mutex);
	/*signal the condition variable - there's a new request to handle*/
	rc = pthread_cond_signal(p_cond_var);
}

/**
 * function get_request : gets the first pending request from the 
 *    requests_list. removing it from the list
 * algorithm: 
 *
 * input: request number, linked list mutex
 * output: pointer to the removed request, or NULL if none
 * memory: the returned request need to be freed  by the caller
 */

struct request * get_request(pthread_mutex_t *p_mutex)  {
	int rc;
	struct request * a_request;

	/* lock the mutex, to assure exclusive access to the list*/
	rc = pthread_mutex_lock(p_mutex);

	if (num_requests > 0) {
		a_request = requests;
		requests = a_request->next;
		if (requests == NULL) {
			last_request = NULL;
		}
		num_requests --;
	} else {
		a_request = NULL;
	}

	rc = pthread_mutex_unlock(p_mutex);

	return a_request;
}

/**
 * function handle_request : handle a single gived request
 * input : request pointer, id of calling thread
 * output : none
 */

void handle_request(struct request * a_request, int thread_id) {
	if ( a_request ) {
		printf("Thread '%d' handled request '%d' \n", thread_id, a_request->number);
		fflush(stdout);
	}
}

/**
 * function handle_requests_loop() ; infinite loop of requests handling
 * input : id of thread
 * output : none
 */

void * handle_requests_loop(void *data) {
	int rc;
	struct request * a_request;
	int thread_id  = (* (int *) data);

#ifdef DEBUG
	printf("Starting thread '%d'", thread_id);
	fflush(stdout);
#endif

	/* lock the mutex, to access the requests list exclusively */
	rc = pthread_mutex_lock(&request_mutex);

#ifdef DEBUG
	printf("thread '%d' after pthread_mutex_lock\n", thread_id);
	fflush(stdout);
#endif

	while(1) {
	//	rc = pthread_mutex_lock(&request_mutex);
#ifdef DEBUG
		printf("thread '%d', num_requests = %d\n", thread_id, num_requests);
		fflush(stdout);
#endif //DEBUG
		if (num_requests > 0) {
			a_request = get_request(&request_mutex);

			if (a_request) {
				/* unlock mutex - so other thread would be able to handle*/
				/* other request waiting in the queue paralelly*/
				rc = pthread_mutex_unlock(&request_mutex);
				handle_request(a_request, thread_id);
				free(a_request);

				rc = pthread_mutex_lock(&request_mutex);
				//
			} 
		}
		else {
				if (done_creating_requests) {
#ifdef DEBUG
					printf("thread '%d' unlocking mutex before exiting\n", thread_id);
					fflush(stdout);
#endif
					pthread_mutex_unlock(&request_mutex);
#ifdef DEBUG
					printf("thread '%d' exiting\n", thread_id);
					fflush(stdout);
#endif
					pthread_exit(NULL);
				} else {
#ifdef DEBUG
					printf("thread '%d' going to sleep\n", thread_id);
					fflush(stdout);
#endif
#ifdef DEBUG
					printf("thread '%d' before cond wait\n", thread_id);
					fflush(stdout);
#endif
					rc = pthread_cond_wait(&got_request, &request_mutex);
#ifdef DEBUG
					printf("thread '%d' after cond wait\n", thread_id);
					fflush(stdout);
				}
		}
#endif
		//rc = pthread_mutex_unlock(&request_mutex);
	}
}


int  main(int argc, char **argv) {
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&request_mutex, &attr);

	int i;
	int thr_id[NUM_HANDLER_THREADS];
	pthread_t p_threads[NUM_HANDLER_THREADS];

	struct timespec delay;

	for(i = 0; i < NUM_HANDLER_THREADS; i++) {
		thr_id[i] = i;
		pthread_create(&p_threads[i], NULL, handle_requests_loop, (void *)&thr_id[i]);
	}

	/* run a loop that generates requests*/
	for ( i = 0; i < 60; i++ ){
		add_request(i, &request_mutex, &got_request);
		/**
		 * pause execution for a little bit, to allow
		 * other threads to run and handle some requests
		 */
		if(rand() > 3 * (RAND_MAX/4)) {
			delay.tv_sec = 0;
			delay.tv_nsec = 1;
			nanosleep(&delay, NULL);
		}
	}
	{
		int rc;
		rc = pthread_mutex_lock(&request_mutex);
		done_creating_requests = 1;
		rc = pthread_cond_broadcast(&got_request);
		rc = pthread_mutex_unlock(&request_mutex);
	}

	for (i = 0; i < NUM_HANDLER_THREADS; i++) {
		void *thr_retval;
		pthread_join(p_threads[i], &thr_retval);
	}
	printf("Glory, we are done\n");
	return 0;
	

}
