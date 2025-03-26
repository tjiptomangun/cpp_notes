#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc,char *argv[])
{
	int ret;
	errno = 0;
	ret = nice (10);     /* increase our nice by 10 */
	if (ret == -1 && errno != 0)
        	 perror ("nice");
	else
        	 printf ("nice value is now %d\n", ret);
	return 0;

}