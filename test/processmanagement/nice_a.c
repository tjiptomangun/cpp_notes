#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc,char *argv[])
{
	int ret, val;
	/* get current nice value */
	val = nice (0);
	/* we want a nice value of 10 */
        val = 10 - val;
	errno = 0;
	ret = nice (val);
	if (ret == -1 && errno != 0)
        	perror ("nice");
	else
        	printf ("nice value is now %d\n", ret);
}