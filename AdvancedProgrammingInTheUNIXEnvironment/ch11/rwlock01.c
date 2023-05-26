#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
// https://tuxthink.blogspot.com/2013/02/using-read-write-lock-in-pthreads.html
pthread_rwlock_t rwlock;

void *errorLock(char * custStr, int actionErr) {
		fprintf(stderr, "error lock %s %d %s", custStr, actionErr, strerror(actionErr));
		return NULL;
}
void *errorUnLock(char * custStr, int actionErr) {
		fprintf(stderr, "error unlock %s %d %s", custStr, actionErr, strerror(actionErr));
		return NULL;
}

int errorOther(char* actionStr, char *custStr, int actionErr, int toRet) {
		fprintf(stderr, "error %s %s %d %s", actionStr, custStr, actionErr, strerror(actionErr));
		return toRet;
}

void* write1 (void *temp) {
	int ret;
	FILE *file;
	int i = 10;
	do  {
		char str[100] = {0};
		if((ret = pthread_rwlock_wrlock(&rwlock)) != 0) {
			return errorLock("w1", ret);
		}
		printf("\nFile locked, please enter the message\n");
		file = fopen("temp", "w");
		scanf("%s", str);
		fprintf(file, "%s", str);
		fclose(file);
		if ((ret = pthread_rwlock_unlock(&rwlock)) != 0) {
			return errorUnLock("w1", ret); 
		}
		printf ("\nfile unlocked by w1 %d", i);
	} while(i -- );
	return NULL;
}

void* write2 (void *temp) {
	int ret;
	FILE *file;
	int i = 10;
	do  {
		char str[100] = {0};
		if((ret = pthread_rwlock_wrlock(&rwlock)) != 0) {
			return errorLock("w2", ret);
		}
		printf("\nFile locked, please enter the message\n");
		file = fopen("temp", "w");
		scanf("%s", str);
		fprintf(file, "%s", str);
		fclose(file);
		if ((ret = pthread_rwlock_unlock(&rwlock)) != 0) {
			return errorUnLock("w2", ret);
		}
		printf ("\nfile unlocked by w2 %d", i);
	} while(i -- );
	return NULL;
	
}

void* read1(void *temp) {
	int ret;
	FILE *file;
	int i = 20;
	do  {
		char str[100] = {0};
		if((ret = pthread_rwlock_wrlock(&rwlock)) != 0) {
			return errorLock("r1", ret);
		}
		printf("\nOpening file from reading by r1\n");
		file = fopen("temp", "r");
		if (file) {
			fscanf(file, "%s", str);
			printf("r1 %d msg: %s\n", i,str);
			fclose(file);
		}
		if ((ret = pthread_rwlock_unlock(&rwlock)) != 0) {
			return errorUnLock("r1", ret);
		}
	} while(i -- );
	return NULL;
}


void* read2(void *temp) {
	int ret;
	FILE *file;
	char str[100] = {0};
	int i = 20;
	do  {
		if((ret = pthread_rwlock_wrlock(&rwlock)) != 0) {
			return errorLock("r2", ret);
		}
		printf("\nOpening file from reading by r2\n");
		file = fopen("temp", "r");
		if (file) {
			fscanf(file, "%s", str);
			printf("r1 %d msg: %s\n", i, str);
			fclose(file);
		}
		if ((ret = pthread_rwlock_unlock(&rwlock)) != 0) {
			return errorUnLock("r2", ret);
		}
	} while(i -- );
	return NULL;
}

int main() { 
	pthread_t thread_id1,thread_id2,thread_id3,thread_id4;
	int ret;
	void *res;
	if ((ret = pthread_rwlock_init(&rwlock,NULL)) != 0) {
		return errorOther("init rwLock", "main", ret, 1);
	}
	else if ((ret=pthread_create(&thread_id1,NULL,&write1,NULL)) != 0) {
		return errorOther("create thread for w1", "main", ret, 1);
	}
	else if ((ret=pthread_create(&thread_id2,NULL,&read1,NULL)) != 0) {
		return errorOther("create thread for r1", "main", ret, 1);
	} 
	else if ((ret=pthread_create(&thread_id3,NULL,&read2,NULL)) != 0) {
		return errorOther("create thread for r2", "main", ret, 1);
	} 
	else if ((ret=pthread_create(&thread_id4,NULL,&write2,NULL)) != 0)  {
		return errorOther("create thread for w2", "main", ret, 1);
	} else {
	printf("\n Created thread");
	}
	if (( ret = pthread_join(thread_id1,&res)) != 0) {
		return errorOther("join thread w1", "main", ret, 1);
	}
	else if (( ret = pthread_join(thread_id2,&res)) != 0) {
		return errorOther("join thread r1", "main", ret, 1);
	}
	else if (( ret = pthread_join(thread_id3,&res)) != 0) {
		return errorOther("join thread r2", "main", ret, 1);
	} 
	else if (( ret = pthread_join(thread_id4,&res)) != 0) {
		return errorOther("join thread w2", "main", ret, 1);
	}
	else if ((ret = pthread_rwlock_destroy(&rwlock)) != 0) {
		return errorOther("destroy relock ", "main", ret, 1);
	}
	else {
		return 0;
	}
}


