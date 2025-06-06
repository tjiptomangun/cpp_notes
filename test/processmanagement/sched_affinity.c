#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char *argv[])
{
	cpu_set_t set;
	int ret, i;
	CPU_ZERO (&set);
	ret = sched_getaffinity (0, sizeof (cpu_set_t), &set);
	if (ret == -1)
        	perror ("sched_getaffinity");
	for (i = 0; i < CPU_SETSIZE; i++) 
	{
        	int cpu;
        	cpu = CPU_ISSET (i, &set);
        	printf ("cpu=%i is %s\n", i,
                 	cpu ? "set" : "unset");
	}
}
