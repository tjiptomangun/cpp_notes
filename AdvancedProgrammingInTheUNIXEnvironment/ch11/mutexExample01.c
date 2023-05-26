/**
 *source https://code-vault.net/lesson/18ec1942c2da46840693efe9b51eabf6 
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void *routine(){
	for (int i = 0; i < 1000000; i++) {
		pthread_mutex_lock(&mutex);
		mails ++;
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char *argv[]){
	pthread_t p1, p2, p3, p4;
	pthread_mutex_init(&mutex, NULL);
	if (pthread_create(&p1, NULL, &routine, NULL)) {
		return 1;
	}
	if (pthread_create(&p2, NULL, &routine, NULL)) {
		return 2;
	}
	if (pthread_create(&p3, NULL, &routine, NULL)) {
		return 3;
	}
	if (pthread_create(&p4, NULL, &routine, NULL)) {
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
	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %d\n", mails);
	return 0;
}
