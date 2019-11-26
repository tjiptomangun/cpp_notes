#include <utmp.h>
#include <stdio.h>
#define UTMPF "/var/run/utmp"
#define WTMPF "/var/log/wtmp"
#define BTMPF "/var/log/btmp"


int main (int argc, char **argv)
{
	FILE * fp;
	struct utmp r_utmp;
	time_t timex;
	int c = 0;
	char *chartime;


	printf("\n\n\n\n\n");
	printf("=======utmp=============");
	printf("\n\n\n\n\n");
	fp = fopen (UTMPF,"rb")	;
	while(!feof(fp))
	{
		fread(&r_utmp,sizeof(struct utmp), 1, fp);

		timex = (time_t ) r_utmp.ut_tv.tv_sec;	
		chartime = ctime(&timex);
		printf("entry : %s . ", chartime);
		printf("%d.  type : %d . user : %s . pid_t : %d . line : %s ",c, r_utmp.ut_type, r_utmp.ut_user, r_utmp.ut_pid, r_utmp.ut_line);

		printf("id : %s . host : %s . exit_stat : %d . term_stat : %d .  ut_session : %d ut_tv.sec : %d . ut_tv.usec : %d . addr : %d.%d.%d.%d", r_utmp.ut_id, r_utmp.ut_host, r_utmp.ut_exit.e_exit, r_utmp.ut_exit.e_termination, r_utmp.ut_session, r_utmp.ut_tv.tv_sec, r_utmp.ut_tv.tv_usec, r_utmp.ut_addr_v6[3], r_utmp.ut_addr_v6[2], r_utmp.ut_addr_v6[1], r_utmp.ut_addr_v6[0]);

		printf("\n");
		c++;
	}
	fclose(fp);

	printf("\n\n\n\n\n");
	printf("=======wtmp=============");
	printf("\n\n\n\n\n");

	fp = fopen (WTMPF,"rb")	;
	while(!feof(fp))
	{
		fread(&r_utmp,sizeof(struct utmp), 1, fp);
		timex = (time_t ) r_utmp.ut_tv.tv_sec;	
		chartime = ctime(&timex);
		printf("entry : %s . ", chartime);
		printf("%d.  type : %d . user : %s . pid_t : %d . line : %s ",c, r_utmp.ut_type, r_utmp.ut_user, r_utmp.ut_pid, r_utmp.ut_line);
		printf("id : %s . host : %s . exit_stat : %d . term_stat : %d .  ut_session : %d ut_tv.sec : %d . ut_tv.usec : %d . addr : %d.%d.%d.%d", r_utmp.ut_id, r_utmp.ut_host, r_utmp.ut_exit.e_exit, r_utmp.ut_exit.e_termination, r_utmp.ut_session, r_utmp.ut_tv.tv_sec, r_utmp.ut_tv.tv_usec, r_utmp.ut_addr_v6[3], r_utmp.ut_addr_v6[2], r_utmp.ut_addr_v6[1], r_utmp.ut_addr_v6[0]);
		//timex = (time_t ) r_utmp.ut_tv.tv_sec;	
		//chartime = ctime(&timex);
		printf("\n");
		c++;
	}
	fclose(fp);

}
