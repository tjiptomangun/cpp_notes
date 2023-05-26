#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
#include "../apue.h"

#define NHASH 29
#define HASH(fp) (((unsigned long)fp) % NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int	f_count; /*protected by hashlock*/
	pthread_mutex_t f_lock;
	struct foo *f_next;
	int		f_id;
	/* ... more stuff here ... */
};

/**
 * Allocate the object
 */
struct foo * foo_alloc(int id) {
	struct foo *fp;
	int idx;

	if ((fp = malloc(sizeof(struct foo)))) { 
		fp->f_count = 1;
		fp->f_id = id;
		if(pthread_mutex_init(&fp->f_lock, NULL)) {
			free(fp);
			return NULL;
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx]= fp;
		pthread_mutex_lock(&fp->f_lock); /** for what */
		pthread_mutex_unlock(&hashlock);
		/** ... continue initialization .. */
		pthread_mutex_unlock(&fp->f_lock); /** for what */
	}
	return fp;
}

void foo_hold(struct foo*fp) {
	pthread_mutex_lock(&hashlock);
	fp->f_count ++;
	pthread_mutex_unlock(&hashlock);
}

struct foo * foo_find(int id) {
	struct foo *fp = NULL;

	pthread_mutex_lock(&hashlock);
	for(fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			fp->f_count ++;
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return fp;
	
}

void foo_release(struct foo *fp) {
	struct foo *tfp;
	int		idx;

	pthread_mutex_lock(&hashlock);
	if (--fp->f_count == 0) {
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->f_next;
		} else {
			while(tfp->f_next != fp) {
				tfp = tfp->f_next;
			}
			tfp->f_next  = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	}
	else {
		pthread_mutex_unlock(&hashlock);
	}
}


void *thread_consumer(void *pleaseFreeMe) {
/*https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/ThreadArgs.html*/
	int k =  * ((int *) pleaseFreeMe);
	printf("consumer %d\n", k);
	free(pleaseFreeMe);
	struct foo *fp = NULL;
	while(1) {
		int j = 0;
		while(j < 100) {
			for(int i = 0; i < 10; i++) {
				int x = k*10 + i;
				fp = foo_find(k*10 + i);
				if(fp) {
					printf("to release twice %d", x);
					foo_release(fp);
					foo_release(fp);
				}
			}
			j++;
		}
		printf("thread consumer sleep\n");
		sleep(1);
	}


	printf("thread consumer exit");
	pthread_exit((void *) "consumer");
	return NULL;

}
void printj(int *j) {
	printf("hash: ");
	for (int i = 0; i< NHASH; i++) {
		printf("%d ", j[i]);
	}
	printf("\n");
}

void* thread_check_hash_empty(){
	int j[NHASH] = {0};
	int allEmpty = 0;


	while(!allEmpty) {
		printj(j);
		int checker = 1;
		for(int a = 0; a < NHASH; a++) {
			checker &= j[a] == 1;
		}
		allEmpty = checker;
		printf("allempty %d  checker %d\n", allEmpty, checker);
		for (int i = 0; i< NHASH; i++) {
			pthread_mutex_lock(&hashlock);
			if (fh[i] == NULL) {
				j[i] = 1;
			}
			else {
				struct foo* tp = fh[i];

				do {
					//printf("tp id %d  count",tp->f_id, tp->f_count);
					tp = tp->f_next;
				}while (tp);
				//printf("\n");
			}
			pthread_mutex_unlock(&hashlock);
		}
		sleep(1);
	}

}
int main () {
	for (int i = 0; i < 100; i++) {
		if (!foo_find(i)) {
			foo_alloc(i);
		}
	}
	pthread_t worker[10] = 	{0};
	pthread_attr_t tattr;
	pthread_t joiner ;

	for (int j = 0; j < 10; j++) {
		int *k = calloc(1, sizeof(int));
		*k = j;
		if (pthread_attr_init(&tattr)) {
			err_ret("pthread_attr_init");
			free(k);
			j--;
		}
		else if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED)){
			err_ret("pthread_attr_setdetachstate");
			pthread_attr_destroy(&tattr);
			free(k);
			j--;
		}
		else if (pthread_create(&worker[j], NULL, thread_consumer, (void *) k)){
			err_ret("pthread_create");
			pthread_attr_destroy(&tattr);
			free(k);
			j--;
		}
		else {
			pthread_attr_destroy(&tattr);
		}

	}
	if (pthread_attr_init(&tattr)) {
		err_ret("pthread_attr_init");
	}
	else if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE)) {
		err_ret("pthread_attr_setdetachstate");

	}
	else if (pthread_create(&joiner, NULL, thread_check_hash_empty, NULL)){
		err_ret("pthread_create");
	}
	else  if (pthread_join(joiner, NULL)){
		err_ret("pthread_join");

	}

}
