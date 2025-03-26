#include <stdio.h>
#include <time.h>

typedef struct sqldatetime {
    unsigned short		year;			/* e.g. 1992		*/
    unsigned char		month;			/* 0-11				*/
    unsigned char		day_of_week;	/* 0-6  0=Sunday, 1=Monday, ... */
    unsigned short		day_of_year;	/* 0-365			*/
    unsigned char		day;			/* 1-31				*/
    unsigned char		hour;			/* 0-23				*/
    unsigned char		minute;			/* 0-59				*/
    unsigned char		second;			/* 0-59				*/
    unsigned long		microsecond;	/* 0-999999			*/
    
} SQLDATETIME;

#define DECL_DATETIME  \
        struct sqldatetime

void XLTimestampToSql (DECL_DATETIME *pSqlTime, struct timespec *pTimeSpec)
{
	struct tm		*pTM; 
	pTM 					= localtime (&pTimeSpec->tv_sec);
	pSqlTime->year			= (short) (pTM->tm_year + 1900);
	pSqlTime->month			= (unsigned char) pTM->tm_mon;
	pSqlTime->day_of_week	= (unsigned char) pTM->tm_wday;
	pSqlTime->day_of_year	= (unsigned short) pTM->tm_yday;
	pSqlTime->day			= (unsigned char) pTM->tm_mday;
	pSqlTime->hour			= (unsigned char) pTM->tm_hour;
	pSqlTime->minute		= (unsigned char) pTM->tm_min;
	pSqlTime->second		= (unsigned char) pTM->tm_sec;
	pSqlTime->microsecond	= pTimeSpec->tv_nsec / 1000L;
	
} /* End of XLTimestampToSql */

void XLTimeToSql (DECL_DATETIME *pSqlTime, time_t *pTime)
{
	struct tm		*pTM;
	
	pTM 					= localtime (pTime);
	pSqlTime->year			= (short) (pTM->tm_year + 1900);
	pSqlTime->month			= (unsigned char) pTM->tm_mon;
	pSqlTime->day_of_week	= (unsigned char) pTM->tm_wday;
	pSqlTime->day_of_year	= (unsigned short) pTM->tm_yday;
	pSqlTime->day			= (unsigned char) pTM->tm_mday;
	pSqlTime->hour			= (unsigned char) pTM->tm_hour;
	pSqlTime->minute		= (unsigned char) pTM->tm_min;
	pSqlTime->second		= (unsigned char) pTM->tm_sec;
	pSqlTime->microsecond	= 0L;
	
} /* End of XLTimeToSql */

int main(int argc, char **argv)
{
	char *ftime;
	time_t tod;
	time_t PreviousDate = (time_t)0x50F58B10;
	time_t CurrentDate  = (time_t)0x50F6DC90;
	ftime = ctime(&PreviousDate);	
	printf("prevdate : %s",ftime);
	ftime = ctime(&CurrentDate);	
	printf("currdate : %s",ftime);
	time (&tod);
	ftime = ctime(&tod);	
	printf("todtime  : %s",ftime);
	
}
