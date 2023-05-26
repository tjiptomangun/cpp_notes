#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/**
 * There is no way to know if mutex destroyed or not
 * because unitialized mutex or destroyed mutex lock return equal
 * error 22 (invalid argument);
 */
void printError(int errNum, const char* cnst) {
	fprintf(stdout, ":::&&&$$$!!!%s error %d %s\n", cnst, errNum, strerror(errNum));
}

int count = 0;
const char *unlockStr = "unlock";
const char *lockStr = "lock";
char str[1000] = {0};

void* th1Task(void *data) {
	pthread_t tid = pthread_self();
	pthread_mutex_t *mut = (pthread_mutex_t*) data;
	fprintf(stdout, "lock and destroy mut %ld\n", tid);
	for(int i = 0; i< 100; i++) {
		int n = 0;
		if((n = pthread_mutex_lock(mut)) == 0){ 
			count ++; 
			strcat(str, "a");
			int m = 0;
			if((m = pthread_mutex_unlock(mut)) != 0){
				printError(m, unlockStr);
				return NULL;
			}
		}else {
			printError(n, lockStr); 
			return NULL;
		}
	}
	return NULL;
}

void* th2Task(void *data) {
	pthread_t tid = pthread_self();
	pthread_mutex_t *mut = (pthread_mutex_t*) data;
	fprintf(stdout, "lock and destroy mut %ld\n", tid);
	for(int i = 0; i < 100; i++) {
		int n = 0;
		if((n = pthread_mutex_lock(mut)) == 0) {
			count ++;
			int len = strlen(str);
			str[len - 1] = 0;
			int m = 0;
			if((m = pthread_mutex_unlock(mut))!= 0) {
				printError(m, unlockStr);
				return NULL;
			}
		}
		else {	
			printError(n, lockStr); 
			return NULL;
		}
	}
	return NULL;
}


int main(int argc, char **argv) {
	pthread_mutex_t *mut = calloc(0, sizeof(pthread_mutex_t));
	pthread_t p[10];
	if (pthread_mutex_init(mut, NULL)) {
		free(mut);
		return 1;
	}

	if (pthread_create(&p[0], NULL, th1Task, (void *) mut)) {
		return 1;
	}
	if(usleep(1000)) {
		perror("usleep main");
		return 1;
	}
	pthread_mutex_destroy(mut);
	for(int i = 1; i < 10; i++) {
		if (i % 2) {
			if (pthread_create(&p[i], NULL, th2Task,  (void *) mut)) {
				return 1;
			}
		}
		else {
			if (pthread_create(&p[i], NULL, th1Task,  (void *) mut)) {
				return 1;
			}
		}
	}

	for(int i = 0; i < 10; i++) {
		if (pthread_join(p[i], NULL)!= 0){
			return 1;
		}
	}

	free(mut);
	fprintf(stdout, "success with count=%d and string length %ld\n", count, strlen(str));
	return 0;
	
}
