#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main ( int argc, char **argv)
{
	struct timeval tv1, tv2;
	struct timezone tz1, tz2;
	int t_elapsed = 0;

	gettimeofday(&tv1,&tz1);
	usleep(110015);
	gettimeofday(&tv2,&tz2);
	
	t_elapsed  = ((tv2.tv_sec - tv1.tv_sec ) *1000000) + (tv2.tv_usec - tv1.tv_usec);
	printf ("elapsed : %d sec(s) and %d microsec(s)\n",t_elapsed/1000000 , t_elapsed %1000000);

	return 0;
}	
