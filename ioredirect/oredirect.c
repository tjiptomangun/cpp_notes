/**
 * gcc -Wall -ggdb3 ordirect.c -o redirect
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NULL 0


int main (int argc, char **argv)
{
	int fd = 0;
	char * const param[] = {"/bin/ls", NULL};

	fd = open ("outfile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); 

	if (fd < 0)
	{
		perror ("open failed\n");
		exit ( -1 );
	}

	pid_t pid = fork ();

	if (pid < 0)
	{
		perror ("fork failed\n");
		exit (0);
	}
	else if(pid == 0)
	{
		dup2 (fd, STDOUT_FILENO);

		close (fd);
		execvp ("ls", param);
	}
	else
	{
		wait (NULL);
	}
	return 0;
}
