#include <unistd.h>
#include <stdio.h>

int main ()
{
	char * argv[] = { "./XLink.sh",  0 };
	char * envp[] =
	{
		"HOME=/home/henky",
		"PATH=/bin:/usr/bin:/home/henky/c_c++/networking/multisession:",
		"TZ=UTC0",
		"USER=henky",
		"PWD=/home/henky/c_c++/network/multisession",
		"LOGNAME=tarzan",
		0 
	};

	execve (argv[0], &argv[0], envp);
	fprintf (stderr, "Oops!\n");
	return -1;
 
}
