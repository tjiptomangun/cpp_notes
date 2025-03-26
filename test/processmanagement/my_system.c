/*
 * my_system - synchronously spawns and waits for the command
 * "/bin/sh -c <cmd>".
 *
 * Returns -1 on error of any sort, or the exit code from the
 * launched process. Does not block or ignore any signals.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int my_system(const char *cmd);
int main(int argc,char *argv[])
{
	char buffer[256];
	int i;
	int ch;
	buffer[0]='\0';
	if(argc<2)
	{
		printf("arg < 2\n");
		return 0;
	}
	for(i = 0; i < argc - 1; i ++)
	{
		strcat(buffer," ");
		strcat(buffer,argv[i+1]);
	}
	my_system(buffer);
	i = 0;
	while(1==1)
	{
		if(i==0)
		{
			printf("any key to continue :");
			ch = getchar();
		}
		i ++;
		i%=2500000;
	}	
}
int my_system(const char *cmd)
{
	int status;
	pid_t pid;
	
	pid=fork();

	if(pid == -1)
		return -1;

	else if(pid == 0)
	{
		const char *argv[4];

		argv[0]="sh";
		argv[1]="-c";
		argv[2]=cmd;
		argv[3]=NULL;
		execv("/bin/sh",argv);

		return(-1);
	}
	if(waitpid(pid,&status,0)==-1)
		return -1;
	else if(WIFEXITED(status))
		return WEXITSTATUS(status);
	return -1;
}