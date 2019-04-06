#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	pid_t pid;
	if (argc < 2)
	{
		printf ("usage : get_sid PID\n");
		exit (1);
	}
	pid = atoi (argv[1]);
	printf ("pid = %d\n", pid);
	printf ("sessid = %d\n", getsid(pid));
	printf ("groupid = %d\n", getpgid(pid));
	exit (0);	
		
	
}
