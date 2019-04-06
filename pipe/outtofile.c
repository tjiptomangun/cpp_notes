#include <stdio.h>

int pipeline( char *process1, char * filename)
{

	int fd[2];
	pipe ( &fd[0] );
	if (fork () != 0 )
	{//The parent process executes these statements
	
		close (fd [0 ] );
		close ( STDOUT_FILENO );
		dup ( fd [1] );
		execl ( process1, process1, 0);
	}

}
