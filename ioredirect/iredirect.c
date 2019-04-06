/**
 * gcc -Wall -ggdb3 iredirect.c -o iredirect
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
	char * const param[] = {"/bin/cat", NULL};

	fd = open ("infile", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR); 

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
		dup2 (fd, STDIN_FILENO);

		close (fd);
		execvp ("cat", param);
	}
	else
	{
		wait (NULL);
	}
	return 0;
}
