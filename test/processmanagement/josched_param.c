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
	FILE *fp;
	int ret;
	struct sched_param sp;
	
	

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

	MySchedParam.sched_priority = max - 79;
	if (sched_setscheduler (0, SCHED_FIFO, &MySchedParam) == -1)
	{
		printf("[ER] XCPSvrInit set priority\n");
	}

	printf("pid=%d\n",pid);

        ret = sched_getparam(0,&sp);
        if(ret == -1)
	{
		perror("sched_getparam");
		return 1;
	}
	while(1==1)
	{
		printf ("Our priority is %d\n", sp.sched_priority);

	}
}
