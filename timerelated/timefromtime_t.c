#include <stdio.h>
#include <time.h>

int main()
{
    time_t stime;
    time_t now;
    char strstime[26];
    char strnow[26];
    struct tm* tm_stime;
    struct tm* tm_now;

    time(&stime);
    tm_stime = localtime(&stime);

    strftime(strstime, 26, "%Y-%m-%d %H:%M:%S", tm_stime);

	sleep(4);

    time(&now);
    tm_now = localtime(&now);

    strftime(strnow, 26, "%Y-%m-%d %H:%M:%S", tm_now);

	fprintf(stdout, "%s   %s\n", strstime, strnow);


 
    return 0;
}
