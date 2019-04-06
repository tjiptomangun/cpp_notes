#include <unistd.h>
#include <stdio.h>

int main ()
{
	char * argv[] = { "/bin/sh", "-c", "env", 0 };
	char * envp[] =
	{
		"HOME=/home/henky",
		"PATH=/bin:/usr/bin:.:",
		"TZ=UTC0",
		"USER=henky",
		"LOGNAME=tarzan",
		0 
	};

	execve (argv[0], &argv[0], envp);
	fprintf (stderr, "Oops!\n");
	return -1;
 
}
