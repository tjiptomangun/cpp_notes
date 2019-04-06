#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	pid_t pid;
	int i;
	long var_sysconf  = 0 ;

	/* create new process */
	pid = fork ();
	if (pid == -1)
		return -1;
	else if (pid != 0)
		exit (EXIT_SUCCESS);

	/* create new session and process group */	
	if (setsid () == -1)
		return -1;
	
	/* set the working directory to home directory */
	if ( chdir ("/home/henky") == -1)
		return -1;

	/* close all open files -- NR_OPEN is overkill, but works.  */

	/* aix and other unix do not have NR_OPEN, getdtablesize is neither 
         * posix standard. Use sysconf (_SC_OPEN_MAX);
	 */
	var_sysconf = sysconf (_SC_OPEN_MAX);
		
	for (i = 0; i < var_sysconf; i++)
		close (i);

	/* redirect fd's 0, 1, 2 to /dev/null */
	open ("/dev/null", O_RDWR); /* stdin */
	dup (0); /*stdout*/
	dup (0); /*stderr*/

	/* do its daemon thing ... */
	i = 0;
	while (1)
	{
		sleep(1);
		i++;	
	}
	

}
