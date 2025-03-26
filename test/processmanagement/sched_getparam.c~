#include <sched.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	struct sched_param sp;
	int ret;
	ret = sched_getparam (0, &sp);
	if (ret == -1) 
	{
		perror ("sched_getparam");
		return 1;
	}
	printf ("Our priority is %d\n", sp.sched_priority);
}