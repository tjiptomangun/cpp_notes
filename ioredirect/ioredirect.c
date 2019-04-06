/**
 * gcc -Wall -ggdb3 ioredirect.c -o ioredirect
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define NULL (void *)0


/*man dup : uses the lowest unused desc for the new desc*/
/*man pipe: creates a pipe, a unidirectional data channel that can be used for ipc*/

int pipeline(char * process1_path, char *process1_name, char *process2_path, char *process2_name )
{
	int fd[2];
	pipe ( &fd[0] );

	if (fork() != 0)
	{
		/*The parent processes executes these statements. */
		close (fd[0]);			/*process 1 does not need to read from pipe*/
		close (STDOUT_FILENO);		/*prepare for new standard output*/
		dup (fd[1]);			/*set standard output to fd[1]*/
		close (fd[1]);			/*this file desc not needed anymore */
		execl ( process1_path, process1_name , (void *)NULL);
	}
	else
	{
		/*The child process executes these statements. */
		close (fd[1]);			/*process 2 does not need to write to pipe */
		close (STDIN_FILENO);		/*prepare for new standard input*/
		dup (fd[0]);
		close (fd[0]);
		execl (process2_path, process2_name, (void *)NULL);
		
	}
	return 0;


}

int main (int argc, char **argv)
{
	pipeline ("/bin/ls","ls", "/bin/cat","cat");

	exit (0);
}
