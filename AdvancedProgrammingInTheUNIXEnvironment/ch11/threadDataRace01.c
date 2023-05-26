/** https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/ */
#include <stdio.h>
#include <pthread.h>

#define TIMES_TO_COUNT 210000000

#define NC  "\e[0m"
#define YELLOW "\e[33m"
#define BYELLOW "\e[1;33m"
#define RED "\e[31m"
#define GREEN "\e[32m"

void *thread_routine(void *data) {
	pthread_t tid = pthread_self();
	unsigned int *count = (unsigned int *) data;
	unsigned int i;
	printf("%sThread [%ld]: Count at thread start = %u.%s\n", YELLOW, tid, *count, NC);
	i = 0;
	while (i < TIMES_TO_COUNT) {

		(*count) ++;

		i++;
	}

	printf("%sThread [%ld]: Final count = %u.%s\n", YELLOW, tid, *count, NC);
	return NULL;
}
int main (void) {
	pthread_t tid1;
	pthread_t tid2;

	unsigned int count = 0;


	printf("Main: Expected count is %s%u%s\n", GREEN, 2*TIMES_TO_COUNT, NC);
	pthread_create(&tid1, NULL, thread_routine, &count);
	printf("Main: Created first thread [%ld]\n", tid1);
	pthread_create(&tid2, NULL, thread_routine, &count);
	printf("Main: Created second thread [%ld]\n", tid2);

	pthread_join(tid1, NULL);
	printf("Main: Join first thread [%ld]\n", tid1);

	pthread_join(tid2, NULL);
	printf("Main: Join second thread [%ld]\n", tid2);

	if (count != (2*TIMES_TO_COUNT)) {
		printf("Main: Error! Total count is %s%u%s\n", RED, count, NC);
	}
	else {
		printf("Main: OK! Total count is %s%u%s\n", GREEN, count, NC);
	}

	return 0;

}
