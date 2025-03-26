#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct sched_param		MySchedParam;
	pid_t pid;
	int policy;
	FILE *fp;
	int ret;
	struct sched_param sp;
	int nIncrementFactor =0;
	if(argc < 3)
	{
		printf("usage : %s pid priority\n");
		exit(0);
	}	

	pid = atoi(argv[1]);
	nIncrementFactor =atoi(argv[2]);
	printf("prio inc = %d\n",nIncrementFactor );


	int min, max;
	min = sched_get_priority_min (SCHED_FIFO);
	if (min == -1) {
		perror ("sched_get_priority_min");
		return 1;
	}
	max = sched_get_priority_max (SCHED_FIFO);
	if (max == -1) {
		perror ("sched_get_priority_max");
		return 1;
	}
	printf ("SCHED_FIFO priority range is %d - %d\n", min, max);	

	MySchedParam.sched_priority =min+nIncrementFactor;
	if (sched_setscheduler (pid, SCHED_FIFO, &MySchedParam) == -1)
	{
		printf("[ER] set priority\n");
		exit(0);
	}
	ret = sched_getparam (pid, &sp);
	printf ("%d priority is %d\n",pid ,sp.sched_priority);	
}