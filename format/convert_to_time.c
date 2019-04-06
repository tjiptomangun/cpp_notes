#include <stdio.h>

/**
 * Name         : convert_to_time_in_millis
 * Description  : convert string to milliseconds
 * Input
 *      strin   : input string, should be in hh:mm:ss.fff
 * Output       : milliseconds from 00:00:00.000
 */
int convert_to_time_in_millis (char *strin)
{
        int ret = ((strin[0] - '0')*36000000) + ((strin[1] - '0')*3600000) +
                ((strin[3] - '0')*600000) + ((strin[4] - '0')*60000)+
                ((strin[6] - '0')*10000) + ((strin[7] -'0')*1000)+
                ((strin[9] - '0')*100) +((strin[10]-'0')*10) + (strin[11]-'0');
        return ret;
}
int usage (char *appname)
{
	fprintf (stdout, "%s input in hh:mm:ss.fff format\n", appname);	
	return 0;
}
int main (int argc, char **argv)
{
	int converted = 0;
	if (argc < 2)
	{
		usage(argv[0]);
		exit (-1);
	}	
	converted  = convert_to_time_in_millis (argv[1]);
	fprintf (stdout, "converted = %d\n", converted);
	return 0;
} 
