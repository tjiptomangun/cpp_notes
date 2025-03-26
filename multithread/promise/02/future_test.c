#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "future.h"
#include <assert.h>
#include <string.h>
typedef ftr_of(int) int_future_t;
// int run_async_task(void *arg) {
// 	int_future_t *fut = arg;
// 	thrd_sleep(&(struct  timespec) (.tv_sec = 2), NULL)
// 	int err = ftr_complete(fut, 42);
// 	if(err) {

// 	}
// }
typedef ftr_of(int16_t) int16_future_t;

bool ftr_test_valuesize() {
	printf("ftr_test_valuesize\n");
	int16_future_t *fut = ftr_new(int16_future_t);
	printf("fut->header.value_size = %lu, sizeof = %lu\n", fut->header.value_size, sizeof(int16_t));
	assert(fut->header.value_size == sizeof(int16_t));
	ftr_delete(fut);
	return true;
}

bool ftr_test_twice() {
    printf("ftr_test_twice\n");
    int_future_t *fut = ftr_new(int_future_t);
    ftr_complete(fut, 42);

    int result;

    int err = ftr_get(fut, 10 * 1000, &result);
    if (err) {
        fprintf(stderr, "error getting future: %s\n", ftr_errorstr(err));
        ftr_delete(fut);
        return false;
    }
    err = ftr_complete(fut, 100);
    assert(err == ftr_invalid);

    err = ftr_get(fut, 10*1000, &result);

    assert(err == ftr_invalid);

    ftr_delete(fut);
    return true;
}

bool ftr_test_samethread() {
    printf("ftr_test_samethread\n");
    int_future_t* fut = ftr_new(int_future_t);

    assert(fut);
    assert(fut->in_value == 0 || !"value is zero initialized");
    assert(fut->out_value == 0 || !"value is zero initialized");
    assert(fut->header.is_valid || !"is valid");

    ftr_complete(fut, 42);

    int result;

    int err = ftr_get(fut, 10*1000, &result);
    if (err) {
        fprintf(stderr, "error getting future: %s\n", ftr_errorstr(err));
        ftr_delete(fut);
        return false;
    }

    assert(result == 42);
    return true;
}
int run_async_task(void *arg) {
    int_future_t * fut = (int_future_t *) arg;
    thrd_sleep(&(struct timespec){.tv_sec = 2}, NULL);
    int err = ftr_complete(fut, 42);
    if (err) {
        fprintf(stderr, "error completing future: %s\n", ftr_errorstr(err));
        return 1;
    }
    return 0;
}

bool ftr_test_success() {
    printf("ftr_test_success\n");
    int_future_t *fut = ftr_new(int_future_t);
    thrd_t thread = {0};

    thrd_create(&thread, run_async_task, fut);

    int result;
    int err = ftr_get(fut, 4 * 1000, &result);
    assert(err == ftr_success);
    if (err) {
        fprintf(stderr, "error writing future: %s\n", ftr_errorstr(err));
        thrd_join(thread, NULL);
        ftr_delete(fut);
        return false;
    }

    assert(result == 42);
    thrd_join(thread, NULL);
    ftr_delete(fut);
    return true;
}

int run_async_task_veryslow(void *arg) {
    int_future_t * fut = arg;
    thrd_sleep(&(struct timespec) {.tv_sec = 5}, NULL);
    int err = ftr_complete(fut, 42);
    if (err) {
        fprintf(stderr, "error completing future: %s \n", ftr_errorstr(err));
        return 1;
    }
    return 0;
}

bool ftr_test_timedout() {
    printf("ftr_test_timedout\n");

    int_future_t* fut = ftr_new(int_future_t);

    thrd_t thread = {0};

    thrd_create(&thread, run_async_task_veryslow, fut);

    int result;
    int err = ftr_get(fut, 4 * 1000, &result);
    assert(err == ftr_timedout);
    if (err) {
        fprintf(stderr, "error writing future: %s\n", ftr_errorstr(err));
        thrd_join(thread, NULL);
        ftr_delete(fut);
        return true;
    }

    assert(result == 42);
    thrd_join(thread, NULL);
    ftr_delete(fut);
    return false;
}
bool ftr_test_tryagain() {
    printf("ftr_test_tryagain\n");
    int_future_t* fut = ftr_new(int_future_t);

    thrd_t thread = {0};

    thrd_create(&thread, run_async_task_veryslow, fut);

    int result = 0;
    int err = ftr_get(fut, 4 * 1000, &result);
    assert(err == ftr_timedout);
    assert(result == 0);
    thrd_sleep(&(struct timespec){.tv_sec = 2}, NULL);

    err = ftr_get(fut, 4 * 1000, &result);
    assert(err == ftr_success);
    assert(result == 42);
    thrd_join(thread, NULL);
    ftr_delete(fut);
    return true;
}

bool ftr_test_wait() {
    printf("ftr_test_wait\n");
    int_future_t* fut = ftr_new(int_future_t);

    thrd_t thread = {0};

    thrd_create(&thread, run_async_task_veryslow, fut);

    int result = 0;
    int err = ftr_timedout;
    while(err == ftr_timedout) {
        err = ftr_wait(fut, 0);
        if (err != ftr_timedout  && err  != ftr_success) {
            thrd_join(thread, NULL);
            ftr_delete(fut);
            return false;
        }
        thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
    }

    err = ftr_get(fut, 4* 1000, &result);
    assert(err == ftr_success);
    assert(result == 42);

    thrd_join(thread, NULL);
    ftr_delete(fut);
    return true;

}

struct data {
    char name[64];
    int x;
    int y;
};

typedef ftr_of(struct data) data_future_t;

int run_async_task_struct(void *arg) {
    data_future_t * fut = arg;
    thrd_sleep(&(struct timespec){.tv_sec = 2}, NULL);

    struct data mydata = {"foobar", 200, 400};
    int err = ftr_complete(fut, mydata);
    if (err) {
        fprintf(stderr, "error completing future: %s\n", ftr_errorstr(err));
        return 1;
    }
    return 0;
}

bool ftr_test_struct() {
    printf("ftr_test_struct\n");
    data_future_t *fut = ftr_new(data_future_t);
    thrd_t thread = {0};
    struct data result;
    thrd_create(&thread, run_async_task_struct, fut);

    int err = ftr_get(fut, 4 * 1000, &result);

    assert(err == ftr_success);

    if (err) {
        fprintf(stderr, "error waiting future: %s\n", ftr_errorstr(err));
        thrd_join(thread, NULL);
        ftr_delete(fut);
        return false;
    }
    assert(!strcmp(result.name, "foobar"));
    assert(result.x == 200);
    assert(result.y == 400);

    thrd_join(thread, NULL);
    ftr_delete(fut);
    return true;

}
bool ftr_runtest() {
    int k = 10;
    int testFunct() {
        k = k*2+1;
    }
    testFunct();
    printf("k is %d\n", k)
    return (
        ftr_test_valuesize() &&
         ftr_test_twice() &&
         ftr_test_samethread() && 
         ftr_test_success() &&
         ftr_test_timedout() &&
         ftr_test_tryagain() && 
         ftr_test_wait() &&
         ftr_test_struct()
	);
}


int main (int argc, char **argv) {
    
    if (ftr_runtest()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
