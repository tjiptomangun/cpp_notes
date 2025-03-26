#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * $CC -Wall -ggdb3 timetm.c -o timetm
 */

int usage ()
{
	fprintf (stderr, "usage timetm lowerlimit upperlimit\n");
	fprintf (stderr, "limits format  YYYYMMDDHHMMSS");
	return 0;
}
/**
 * filltm
 * filltm structure from string
 *
 **/
int filltm (char *in, struct tm *inout)
{
	char *pin;
	pin  = &in[0];
	inout->tm_year = (pin[0] - '0')*1000 + (pin[1] - '0')*100 + 
			(pin[2] - '0')*10+ (pin[3] - '0') - 1900;
	printf ("%04d", inout->tm_year);

	pin  = &in[4];
	inout->tm_mon  = (pin[0] - '0') * 10 + (pin[1] - '0') - 1;
	printf ("%02d", inout->tm_mon);

	pin  = &in[6];
	inout->tm_mday = (pin[0] - '0') * 10 + (pin[1] - '0');	
	printf ("%02d", inout->tm_mday);
	
	pin  = &in[8];
	inout->tm_hour = (pin[0] - '0') * 10 + (pin[1] - '0');	
	printf ("%02d", inout->tm_hour);

	pin  = &in[10];
	inout->tm_min = (pin[0] - '0') * 10 + (pin[1] - '0');	
	printf ("%02d", inout->tm_min);

	pin  = &in[12];
	inout->tm_sec = (pin[0] - '0') * 10 + (pin[1] - '0');	
	printf ("%02d\n", inout->tm_sec);

	inout->tm_isdst  = -1;	
	return 0;
}
int verbosetime (char *varname, struct tm *tmn)
{	
	printf ("%s :%02d%02d%02d%02d%02d%02d\n", varname, tmn->tm_year-100, 
		tmn->tm_mon, tmn->tm_mday, tmn->tm_hour, tmn->tm_min, tmn->tm_sec);
	return 0;
}
int main ( int argc, char *argv[] )
{
	time_t now;
	struct tm tmnow;
	struct tm tmlo, tmup;
	time_t tlo, tup;
	if (argc < 3)
	{
		usage ();
		exit (EXIT_FAILURE);

	}
	if(strlen (argv[1]) != 14)
	{
		usage();	
		exit (EXIT_FAILURE);

	}
	if(strlen (argv[2]) != 14)
	{
		usage();	
		exit (EXIT_FAILURE);

	}

	fprintf (stdout,"argv[1] =%s\n", argv[1]);
	fprintf (stdout,"argv[2] =%s\n", argv[2]);
	
	time ( &now );
	gmtime_r ( &now, &tmnow ); 
	verbosetime ("gmtime", &tmnow); 

	localtime_r ( &now, &tmnow); 
	verbosetime ("localtime", &tmnow);

	filltm (argv[1], &tmlo)	;
	filltm (argv[2], &tmup)	;

	tlo = mktime (&tmlo);
	if (tlo == -1)
	{
		fprintf (stderr, "lowerlimit unknown\n");
		exit (EXIT_FAILURE);
	}
	verbosetime ("tmlo", &tmlo);
	tup = mktime (&tmup);
	if (tup == -1)
	{
		fprintf (stderr, "upperlimit unknown\n");
		exit (EXIT_FAILURE);
	}
	verbosetime ("tmup", &tmup);

	if ( ((int) difftime(tup, now) <=0) || 
	     ((int) difftime(now, tlo) <=0))
	{
		fprintf (stdout, "expired license\n"); 
	}
	else
	{
		fprintf (stdout, "license active\n"); 
	}

	exit (EXIT_SUCCESS);

}
