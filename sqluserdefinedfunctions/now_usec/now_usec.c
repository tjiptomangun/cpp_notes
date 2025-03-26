//Schwartz, Baron_Tkachenko, Vadim_Zaitsev, Peter - High performance MySQL_ optimization, backups, and replication-O'Reilly Media (2012).pdf
//https://github.com/mysqludf
//https://www.uv.es/jordi/v3/mysql/manual/manual_Adding_functions.html
//https://stackoverflow.com/a/55245524
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32)
#define DLLEXP __declspec(dllexport) 
#else
#define DLLEXP
#endif
//#ifdef HAVE_DLOPEN
#ifdef	__cplusplus
extern "C" {
#endif

	DLLEXP my_bool now_usec_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	DLLEXP char *now_usec(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error); 
	DLLEXP my_bool now_usec_deinit(UDF_INIT *initid);
#ifdef	__cplusplus
}
#endif

my_bool now_usec_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return 0;
}
DLLEXP my_bool now_usec_deinit(UDF_INIT *initid) {
}
char *now_usec(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
	struct timeval tv;
	struct tm *ptm;
	char time_string[20] = {0};
	char *usec_time_string = result;
	time_t t;

	gettimeofday(&tv, NULL);
	t = (time_t) tv.tv_sec;

	ptm = localtime(&t);
	strftime(time_string, sizeof(time_string), "%Y%m%d%H%M%S", ptm);
	sprintf(usec_time_string, "%s.%06ld\n", time_string, tv.tv_usec);
	*length = 20;
	return (usec_time_string);
}
//#endif
