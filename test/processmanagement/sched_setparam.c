#include <sched.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct sched_param sp;
	struct timespec tp;
	int ret,policy,pid;
	int min, max;
	min = sched_get_priority_min (SCHED_RR);
	if (min == -1) {
		perror ("sched_get_priority_min");
		return 1;
	}
	max = sched_get_priority_max (SCHED_RR);
	if (max == -1) {
		perror ("sched_get_priority_max");
		return 1;
	}
	printf ("SCHED_RR priority range is %d - %d\n", min, max);
	pid = getpid();
	policy = sched_getscheduler (pid);
	if (policy == -1)
        	return -1;

	memset (&sp, 0, sizeof (struct sched_param));
	sp.sched_priority = max;
	policy = sched_setparam (pid, &sp);
	if (ret == -1)
	 {
        	perror ("sched_setparam");
        	return 1;
	}
	printf ("Our priority set to %d\n", sp.sched_priority);

	ret = sched_rr_get_interval(0,&tp);
	if(ret == -1)
	{
		perror ("sched_rr_get_interval");
		return 1;
	}
	/* convert the seconds and nanoseconds to milliseconds */
	printf ("Our time quantum is %.2lf milliseconds\n",
        	(tp.tv_sec * 1000.0f) + (tp.tv_nsec / 1000000.0f));

}