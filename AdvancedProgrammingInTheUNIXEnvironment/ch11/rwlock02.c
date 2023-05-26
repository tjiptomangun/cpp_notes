// https://stackoverflow.com/questions/19482648/reader-writer-lock-in-pthread
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
volatile int x = 0;

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

pthread_rwlock_t lock_rw = PTHREAD_RWLOCK_INITIALIZER;

void *reader_thread(void *arg) {
	int i;
	int ret = 0;
	int newx, oldx;
	newx = oldx = -1;

	pthread_rwlock_t *p = (pthread_rwlock_t *)arg;

	for (i = 0; i < 1000; i++) {
		if ((ret  = pthread_rwlock_rdlock(p)) != 0) {
			return errorLock( "reader", ret);
		}
		newx = ACCESS_ONCE(x);
		if (oldx != newx) {
			printf ("reader lock: x : %d\n", x);
		}	
		oldx = newx;
		if ((ret  = pthread_rwlock_unlock(p)) != 0) {
			return errorUnLock( "reader", ret);
		}
	}

	return NULL;
}

void *writer_thread(void *arg)
{
    int i;
    pthread_rwlock_t *p = (pthread_rwlock_t *)arg;
	int ret = 0;


    for (i = 0; i < 3; i++) {
	    if ((ret = pthread_rwlock_wrlock(p)) != 0) {
			return errorLock( "writer", ret);
	    }
        ACCESS_ONCE(x)++;
	    if ((pthread_rwlock_unlock(p)) != 0) {
			return errorUnLock( "writer", ret);
	    }
    } 
    return NULL;
}
int main(int argc, char **argv) {
    pthread_t tid1, tid2;
	int ret = 0;
    void *vp;
    if ((ret = pthread_create(&tid1, NULL, reader_thread, &lock_rw)) != 0) {
		return errorOther("create thread for r", "main", ret, 1); 
    } 
    if ((ret = pthread_create(&tid2, NULL, writer_thread, &lock_rw)) != 0) {
		return errorOther("create thread for w", "main", ret, 1);
    }

    //wait for the thread to complete
    if ((ret = pthread_join(tid1, &vp)) != 0) {
		return errorOther("join thread r", "main", ret, 1);
    }


    if ((ret = pthread_join(tid2, &vp)) != 0) {
		return errorOther("join thread w", "main", ret, 1);
    }

    printf("Parent process sees x: %d\n",x);
    return 0;
}
