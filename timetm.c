/*
 * cc timetm.c -Wall -ggdb3 -o timetm
 *
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main ( int argc, char **argv )
{
	time_t now;
	struct tm tmnow;
	
	time ( &now );
	gmtime_r ( &now, &tmnow );

	printf ("%02d%02d%02d%02d%02d%02d\n", tmnow.tm_year-100, tmnow.tm_mon, tmnow.tm_mday, tmnow.tm_hour, tmnow.tm_min, tmnow.tm_sec);

	exit ( 0);

}
