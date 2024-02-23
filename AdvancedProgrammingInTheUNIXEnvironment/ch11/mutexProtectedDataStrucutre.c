#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../apue.h"
/**
 * https://notes.shichao.io/apue/ch11/
 */
#define NHASH 29
#define HASH(id) (((unsigned long)id) % NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;
//convention hash lock first then f_lock

struct foo{
	int f_count;
	pthread_mutex_t f_lock;
	int f_id;
	struct foo *f_next; /*protected by hashlock*/
	
	/** ...more stuff here ...*/
};

struct foo* foo_alloc(int id) {
	struct foo *fp;
	int idx;
	printf("alloc %d\n", id);
	if((fp =  malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id= id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return NULL;
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		/** continue initialization ...*/
		pthread_mutex_unlock(&fp->f_lock);
	}
	return fp;
}

void foo_hold(struct foo * fp){/*add reference to the object*/
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count ++;
	pthread_mutex_unlock(&fp->f_lock);
}

void foo_reduce(struct foo *fp) {
	printf("reduce %d %d", fp->f_id, fp->f_count);
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count --;
	pthread_mutex_unlock(&fp->f_lock);
}



void foo_release(struct foo *fp) {/** release reference to the object*/
	struct foo *tfp;
	int idx;
	pthread_mutex_lock(&fp->f_lock);//first lock f_lock to check condition
	printf("releasing %d count %d\n", fp->f_id, fp->f_count);
	if (fp->f_count == 1) {
		pthread_mutex_unlock(&fp->f_lock);//unlock so this function do not blocked because next we want to remove from hash
										//and that acquire hash lock .
										//and if we do not unlock f_lock we contradict with
										//foo_find that lock hash lock then lock f_lock
										//so our protocol always lock hash lock first then f_lock
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);
		/**
         * need to recheck the condition
		 * because while waiting for hash lock lots of thing can happen
		*/
		if (fp->f_count != 1) {
			fp->f_count --;
			pthread_mutex_unlock(&fp->f_lock);
			pthread_mutex_lock(&hashlock); 
			return ;
		}
		/* remove from list */
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp){
			fh[idx] = fp->f_next;
		}
		else {
			while(tfp->f_next != fp){
				tfp = tfp->f_next;
			}
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		fp->f_count --;
		pthread_mutex_unlock(&fp->f_lock);
	}
}

struct foo* foo_find(int id) {
	struct foo *fp;
	pthread_mutex_lock(&hashlock);
	//printf("foo_find %d hash %p next %p id count %d \n", id, fh[HASH(id)], fh[HASH(id)] ? fh[HASH(id)]->f_next : 0, fh[HASH(id)] ? fh[HASH(id)]->f_count : -1);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			foo_hold(fp);
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	if (fp) {
		//printf("found %p\n", fp);
	}
	else {
		//printf("found NULL pointer\n");
	}

	return (fp);
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
	return NULL;

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
