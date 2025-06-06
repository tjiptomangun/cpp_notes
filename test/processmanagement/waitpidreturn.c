#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main (void)
{
	int status;
	pid_t pid;
	pid_t pid2;
	pid = fork();
	int n = 0;
	if(!pid)
	{
		
		while(n<10)
		{
			sleep(10);
			n++;
		}
		return 1;
	}
	pid2 = waitpid(pid,&status,WNOHANG);

	if(pid2==-1)
		perror("waitpid");
	else
	{
		printf("pid = %d\n",pid2);

		if(WIFEXITED(status))
			printf("Normal termination with exit status=%d\n",WEXITSTATUS(status));
		if(WIFSIGNALED(status))
			printf("Killed by signal=%d%s\n",
				WTERMSIG(status),
				WCOREDUMP(status) ? " (dumped core)":"");
	}
	
}