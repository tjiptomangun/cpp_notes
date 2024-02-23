// https://linuxhint.com/https-linuxhint-com-posix-semaphores-with-c-programming/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t s;

void *T(void *arg) {
	int *id = (int *) arg;
	sem_wait(&s);
	printf("Welcome thread # %d!\n", *id);
	sleep(4);
	printf("Bye!\n");
	sem_post(&s);
	return NULL;
}

int main() {
	sem_init(&s, 0, 1);
	pthread_t o1, o2;
	int threadNum1 = 1, threadNum2 = 2;
	pthread_create(&o1, NULL, T, &threadNum1);
	sleep(4);
	pthread_create(&o2, NULL, T, &threadNum2);
	pthread_join(o1, NULL);
	pthread_join(o2, NULL);
	sem_destroy(&s);
}
