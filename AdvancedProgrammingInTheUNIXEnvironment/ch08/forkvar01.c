#include "../apue.h"

int globvar = 6; /* external variable in initialized data */
char buf[]= "a write to stdout \n";

int main(void) {
	int var;
	pid_t pid;

	var = 88;

	if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf)  - 1){
		err_sys("write_error");
	}

	printf("before fork\n"); /*we don't flush stdout*/

	if ((pid = fork()) < 0) {
		err_sys("fork_error");
	} else if (pid == 0) {
		globvar ++; /* child, modify variables */
		var ++;
	} else {
		sleep(2);/* parent */
	}

	printf("lpid = %d pid = %ld, glob = %d ,var = %d\n", pid, (long)getpid(), globvar, var);
	exit(0);
}
