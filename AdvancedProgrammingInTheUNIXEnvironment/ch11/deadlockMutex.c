/**
 *source https://code-vault.net/lesson/18ec1942c2da46840693efe9b51eabf6
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>



#define NC  "\e[0m"
#define YELLOW "\e[33m"
#define BYELLOW "\e[1;33m"
#define RED "\e[31m"
#define GREEN "\e[32m"


typedef struct s_locks {
	pthread_mutex_t lock_1;
	pthread_mutex_t lock_2;
	unsigned int count;
} t_locks;

void *thread_1_routine(void *data) {
	pthread_t tid = pthread_self();
	t_locks *locks = (t_locks *) data;

	for(int i = 0; i < 1; i++) {
		printf("%sThread [%ld]: wants to lock 1%s\n", YELLOW, tid, NC);
		pthread_mutex_lock(&locks->lock_1);
		printf("%sThread [%ld]: owns lock 1%s\n", BYELLOW, tid, NC);
		printf("%sThread [%ld]: wants to lock 2%s\n", YELLOW, tid, NC);
		pthread_mutex_lock(&locks->lock_2);
		printf("%sThread [%ld]: owns lock 2%s\n", BYELLOW, tid, NC);
		locks->count += 1;
		printf("%sThread [%ld]: unlocking lock 2%s\n", BYELLOW, tid, NC);
		pthread_mutex_unlock(&locks->lock_2);
		printf("%sThread [%ld]: unlocking lock 1%s\n", BYELLOW, tid, NC);
		pthread_mutex_unlock(&locks->lock_1);
	}

	return NULL;
}

void *thread_2_routine(void *data) {
		pthread_t tid = pthread_self();
		t_locks *locks = (t_locks *)data;

		for(int i = 0; i < 1; i++) {
			printf("%sThread [%ld]: wants to lock 2%s\n", YELLOW, tid, NC);
			pthread_mutex_lock(&locks->lock_2);
			printf("%sThread [%ld]: owns lock 2%s\n", BYELLOW, tid, NC);
			printf("%sThread [%ld]: wants to lock 1%s\n", YELLOW, tid, NC);
			pthread_mutex_lock(&locks->lock_1);
			printf("%sThread [%ld]: owns lock 1%s\n", BYELLOW, tid, NC);
			locks->count -= 1;
			printf("%sThread [%ld]: unlocking lock 1%s\n", BYELLOW, tid, NC);
			pthread_mutex_unlock(&locks->lock_1);
			printf("%sThread [%ld]: unlocking lock 2%s\n", BYELLOW, tid, NC);
			pthread_mutex_unlock(&locks->lock_2);
		}

		return NULL;
}

int main (void) {
	pthread_t tid1;
	pthread_t tid2;
	t_locks locks;

	locks.count = 0;
	pthread_mutex_init(&locks.lock_1, NULL);
	pthread_mutex_init(&locks.lock_2, NULL);

	pthread_create(&tid1, NULL, thread_1_routine, &locks);
	printf("Main: Created first thread [%ld]\n", tid1);
	pthread_create(&tid2, NULL, thread_2_routine, &locks);
	printf("Main: Created second thread [%ld]\n", tid2);

	pthread_join(tid1, NULL);
	printf("Main: Join first thread [%ld]\n", tid1);

	pthread_join(tid2, NULL);
	printf("Main: Join second thread [%ld]\n", tid2);

	if (locks.count != 0) {
		printf("Main: Error! Total count is %s%u%s\n", RED, locks.count, NC);
	}
	else {
		printf("Main: OK! Total count is %s%u%s\n", GREEN, locks.count, NC);
	}

	pthread_mutex_destroy(&locks.lock_1);
	pthread_mutex_destroy(&locks.lock_2);
	return 0;


}
