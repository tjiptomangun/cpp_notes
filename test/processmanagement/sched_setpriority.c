#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[])
{
	struct sched_param		MySchedParam;
	pid_t pid;
	int policy;
	FILE *fp;
	int ret;
	struct sched_param sp;
	int nIncrementFactor =0;
        /*
	pid_t ppid = getppid();
	printf("bash ppid of %d\n",ppid);
	policy = sched_getscheduler (ppid);
	switch (policy) {
	case SCHED_OTHER:
		printf ("Policy is normal\n");
		break;
	case SCHED_RR:
		printf ("Policy is round-robin\n");
		break;
	case SCHED_FIFO:
		printf ("Policy is first-in, first-out\n");
		break;
	case -1:
		perror ("sched_getscheduler");
		break;
	default:
		fprintf (stderr, "Unknown policy!\n");
        }
        */
	if(argc >1)
	{				
		nIncrementFactor =atoi(argv[1]);
		printf("prio inc = %d\n",nIncrementFactor );
	}

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

/*
	MySchedParam.sched_priority =max - 1;
	printf("setting parent priority\n");
	if (sched_setscheduler (ppid, SCHED_FIFO, &MySchedParam) == -1)
	{
		printf("[ER] set priority\n");
		exit(0);
	}
*/
        memset(&MySchedParam,0,sizeof(struct sched_param ));
	MySchedParam.sched_priority =nIncrementFactor;
	if (sched_setscheduler (0, SCHED_FIFO, &MySchedParam) == -1)
	{
		printf("[ER] set priority\n");
		exit(0);
	}
	ret = sched_getparam (0, &sp);
	printf ("Our priority is %d\n", sp.sched_priority);	
	ret = 0;	
        while (1==1)
	{
		printf("ret = %d\n",ret);
		ret++;
		ret%=10000;
	}
}
