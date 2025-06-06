#include "../apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 8					/*# of threads*/
#define NUMNUM 800000L			/*# of numbers to sort*/ 
#define TNUM (NUMNUM/NTHR)		/*# to sort per thread*/

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t, int (*) (const  void *, const void *));
#endif

/**
 * compare two long integers
 */
int complong(const void *arg1, const void *arg2) {
	long l1 = *(long *)arg1;
	long l2 = *(long *)arg2;

	if (l1 == l2) {
		return 0;
	} else if (l1 < l2) {
		return -1;
	} else {
		return 1;
	}
}

/**
 * worker thread to sort a portion set of numbers
 */

void * thread_fn(void *arg) {
	long idx = (long) arg;

	heapsort(&nums[idx], TNUM, sizeof(long), complong);
	pthread_barrier_wait(&b);
	return ((void *) 0);
}

/**
 * Merge the results of the individual sorted ranges
 */
void merge() {
	long idx[NTHR];
	long i, minidx, sidx, num;

	for (i = 0; i < NTHR; i++) {
		idx[i] = i * TNUM;
	}

	for (sidx = 0; sidx < NUMNUM; sidx ++) {
		num = LONG_MAX;
		for (i = 0; i < NTHR; i++) {
			if ((idx[i] < (i+1) * TNUM) && (nums[idx[i]]  < num)) {
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}

int main() {
	unsigned long i;
	struct timeval start, end;
	long long startusec, endusec;
	double elapsed;
	int	err;
	pthread_t tid;

	/**
	 * Create the initial set of numbers to sort
	 */
	srandom(1);
	for (i = 0; i < NUMNUM; i++) {
		nums[i] = random();
	}

	/**
	 * Create 8 thread to sort the numbers
	 */
	gettimeofday(&start, NULL);
	pthread_barrier_init(&b, NULL, NTHR + 1);
	for (i = 0; i < NTHR; i++) {
		err = pthread_create(&tid, NULL, thread_fn, (void *) (i * TNUM));
		if (err != 0) {
			err_exit(err, "can't create thread");
		}
	}
	pthread_barrier_wait(&b);
	merge();
	gettimeofday(&end, NULL);

	startusec = start.tv_sec * 1000000 + start.tv_usec;
	endusec = end.tv_sec * 1000000 + end.tv_usec;
	elapsed = (double)(endusec - startusec)/1000000.0;
	printf("sort took %.4f seconds\n", elapsed);
	for (i = 0; i< NUMNUM; i++) {
		printf(" %ld ", snums[i]);
	}
	printf("sort took %.4f seconds\n", elapsed);
	exit(0);
}
