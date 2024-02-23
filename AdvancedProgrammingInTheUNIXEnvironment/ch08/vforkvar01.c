#include "../apue.h"

int globvar = 6; /* external variable in initialized data */

int main(void) {
	int var;
	pid_t pid;

	var = 88;


	//printf("before vfork\n"); /*we don't flush stdout*/

	if ((pid = vfork()) < 0) {
		err_sys("vfork_error");
	} else if (pid == 0) {
		globvar ++; /* child, modify variables */
		var ++;
		_exit(0);
	} 
	

	printf("lpid = %d pid = %ld, glob = %d ,var = %d\n", pid, (long)getpid(), globvar, var);
	exit(0);
}
