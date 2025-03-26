#include <sys/types.h>
#include <unistd.h>

int main(int argc ,char *argv[])
{
	printf("My Pid = %d\n",getpid());
	printf("Parent's Pid = %d\n",getppid());
	
}