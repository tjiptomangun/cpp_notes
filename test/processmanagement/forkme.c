#include <sys/types.h>
#include <unistd.h>

int main(int argc, char**argv)
{
	pid_t pid;
	pid=fork();
	if(pid>0)
		printf("i am the parent of pid %d!\n",pid);
	else if(!pid)
		printf("i am the baby!\n");
	else if(pid==-1)
		perror("fork");
	sleep(10);
	return 0;
}