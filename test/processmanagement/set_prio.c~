#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char *argv[])
{
	int ret;
	ret = getpriority (PRIO_PROCESS, 0);
	printf ("nice value is %d\n", ret);

	ret = setpriority (PRIO_PGRP, 0, 10);
	if (ret == -1)
        	perror ("setpriority");


}