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

	CPU_ZERO (&set);   /* clear all CPUs */
	CPU_SET (0, &set); /* allow CPU #0 */
	CPU_CLR (1, &set); /* forbid CPU #1 */

	ret = sched_setaffinity (0, sizeof (cpu_set_t), &set);
	if (ret == -1)
        	perror ("sched_setaffinity");
	for (i = 0; i < CPU_SETSIZE; i++) 
	{
        	int cpu;
        	cpu = CPU_ISSET (i, &set);
        	printf ("cpu=%i is %s\n", i,
                	 cpu ? "set" : "unset");
	}


}