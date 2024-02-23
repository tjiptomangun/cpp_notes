//https://code-vault.net/lesson/18ec1942c2da46840693efe9b520b377
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_barrier_t barrier;

void *routine(void *args) {
	char * p =(char *) args;
	while(1) {
		printf("Waiting at the barrier ...%s\n", p);
		sleep(1);
		pthread_barrier_wait(&barrier);
		printf("We passed the barrier ...%s\n", p);
		sleep(1);
	}
}

int main(int argc, char **argv) {
	pthread_t th[10];
	char * ser1 = "ser 1";
	char * ser2 = "ser 2";
	char kname [2][10][100] = {0};


	pthread_barrier_init(&barrier, NULL, 10);

	for (int i = 0; i < 10; i ++) {
		char * ki = kname[0][i];
		sprintf(ki, "%s %d", ser1, i);
		if (pthread_create(&th[i], NULL, &routine, ki)) {
			perror("failed to create thread");
		} 
	}
	for (int i = 0; i < 10; i ++) {
		char * kj = kname[1][i];
		sprintf(kj, "%s %d", ser2, i);
		if (pthread_create(&th[i], NULL, &routine, kj)) {
			perror("failed to create thread");
		} 
	}
}
