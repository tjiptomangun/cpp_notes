/**
 * example of invalid pthred_exit argument
 * by the time main thread call printfoo of thread1 exit argument
 * its argument no more valid because it is a local variable.
 */

#include "../apue.h"
#include <pthread.h>

struct foo {
	int a, b, c, d;
};
void printfoo(const char *s, const struct foo *fp) {
	printf("%s", s);
	printf(" structure at 0x%lx\n", (unsigned long)fp);
	printf(" foo.a = %d\n", fp->a);
	printf(" foo.b = %d\n", fp->b);
	printf(" foo.c = %d\n", fp->c);
	printf(" foo.d = %d\n", fp->d);
}
void* thr_fn1(void *arg) {
	struct foo foo = {1, 2, 3, 4};
	printfoo("thread 1 :\n", &foo);
	//return not working here
	pthread_exit((void *)&foo);
}

void* thr_fn2(void *arg) {
	printf("thread 2 exiting\n");
	pthread_exit((void *)0);
}
int main(void) {
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;
	int tret;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0) {
		err_quit("can't create thread 1: %s\n", strerror(err));
	}
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0) {
		err_quit("can't create thread 2: %s\n", strerror(err));
	}
	err = pthread_join(tid1, (void *)&fp);
	if (err != 0) {
		err_quit("can't join thread 1: %s\n", strerror(err));
	}
	sleep(1);
	printf("parent starting second thread\n");
	err = pthread_join(tid2, (void *)&tret);
	if (err != 0) {
		err_quit("can't join thread 2: %s\n", strerror(err));
	}
	printfoo("parent: \n", fp);
	exit(0);
}
