/*
 * xsyshealth.c
 *
 *  Created on: Mar 17, 2011
 *      Author: henky
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int FileSize(FILE *FInFp);
int AppendTempToLog(FILE *fpLog,FILE *fpTemp,int nLogType,char *szSrc,char *szDst);
int AppendFileToLog(FILE *fpLog,char *fname,int nLogType,char *szSrc,char *szDst);
int ExecuteAndLog(char *system,char *tempfile,FILE *logptr,int logType, char *src,char *dst);
#define MAX_BUFFSIZE 80
#define MAX_SYSTEMSIZE 280
int main(int argc, char *argv[])
{
	time_t                   now;
	struct tm  *ts;
	char       buf[MAX_BUFFSIZE];
	char       fname[MAX_BUFFSIZE];
	char       tname[MAX_BUFFSIZE];
	char       logdir[MAX_BUFFSIZE];
	FILE        * fp=NULL;
	int         bHelp=0,opt;
	logdir[0]='\0';
	strcpy(logdir,"../log/");

    while ((opt = getopt(argc, argv, "hd:")) != -1) {
        switch (opt) {
        case 'h':
        	bHelp = 1;
            break;
        case 'd':
        	logdir[0]='\0';
        	strcpy(logdir,optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-h] [-d] logdirectory\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
	if(bHelp==1)
	{
#ifdef _RHEL_AS4_
	printf("vmstat legend:\n");
	printf("r: The number of processes waiting for run time\n");
	printf("b: The number of processes in uninterruptible sleep.\n");
	printf("swpd: the amount of virtual memory used.\n");
	printf("free: the amount of idle memory.\n");
	printf("buff: the amount of memory used as buffers.\n");
	printf("cache: the amount of memory used as cache.");
	printf("si: Amount of memory swapped in from disk (/s).\n");
	printf("so: Amount of memory swapped to disk (/s).\n");
	printf("bi: Blocks received from a block device (blocks/s).\n");
	printf("bo: Blocks sent to a block device (blocks/s).\n");
	printf("in: The number of interrupts per second, including the clock.\n");
	printf("cs: The number of context switches per second.\n");
	printf("us: Time spent running non-kernel code. (user time, including nice time)\n");
	printf("sy: Time spent running kernel code. (system time)\n");
	printf("id: Time spent idle. Prior to Linux 2.5.41, this includes IO-wait time.\n");
	printf("wa: Time spent waiting for IO. Prior to Linux 2.5.41, included in idle.\n");
	printf("st: Time stolen from a virtual machine. Prior to Linux 2.6.11, unknown.\n");
	printf("\n\n");
#endif
#ifdef _aix_51_
	printf("vmstat legend:\n");
	printf("kthr: kernel thread state changes per second over the sampling interval.\n");
	printf("r Number of kernel threads placed in run queue.\n");
	printf("b Number of kernel threads placed in wait queue (awaiting resource, awaiting input/output).");
	printf("Memory: information about the usage of virtual and real memory. Virtual pages are considered active if they have been accessed. A page is 4096 bytes.");
	printf("avm Active virtual pages.\n");
	printf("fre Size of the free list.");
	printf("Page: information about page faults and paging activity. These are averaged over the interval and given in units per second.");
	printf("re Pager input/output list.\n");
	printf("pi Pages paged in from paging space.\n");
	printf("po Pages paged out to paging space.\n");
	printf("fr Pages freed (page replacement).\n");
	printf("sr Pages scanned by page-replacement algorithm.\n");
	printf("cy Clock cycles by page-replacement algorithm.\n");
	printf("Faults: trap and interrupt rate averages per second over the sampling interval.");
	printf("in Device interrupts.\n");
	printf("sy System calls.\n");
	printf("cs Kernel thread context switches.\n");
	printf("Cpu: breakdown of percentage usage of CPU time.\n");
	printf("us User time.\n");
	printf("sy System time.\n");
	printf("id CPU idle time\n");
	printf("wa CPU idle time during which the system had outstanding disk/NFS I/O request(s). See detailed description above.\n");
	printf("\n");
#endif
	return 0;
	}


	now = time(NULL);
	ts = localtime(&now);
	tname[0]='\0';



	sprintf(&tname[0],"%sxsyshealth.tmp",logdir);
	sprintf(buf,"%sXSysHealth_%%m%%d",logdir);
	strftime(fname, sizeof(buf), buf, ts);

	fp = fopen((const char *)fname,"a");

	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
	fprintf(fp,"************ Executed %s ************\n", buf);
	ExecuteAndLog("uptime",tname,fp,0,NULL,NULL);
	fprintf(fp,"\ncheck for zombies");
	ExecuteAndLog("ps -ef|grep -i defun | grep -v grep",tname,fp,0,NULL,NULL);
#ifdef _RHEL_AS4_
	ExecuteAndLog("df -H",tname,fp,0,NULL,NULL);
#endif
#ifdef _aix_51_
	ExecuteAndLog("df -k",tname,fp,0,NULL,NULL);
#endif
#ifdef _RHEL_AS4_
	ExecuteAndLog("free",tname,fp,0,NULL,NULL);
#endif
#ifdef _aix_51_
	ExecuteAndLog("lsattr -El `lsdev -C|grep mem | sed -e \'s/^\\([[:alnum:]][[:alnum:]]*\\) *.*/\\1 /g\'`",tname,fp,0,NULL,NULL);
#endif

	ExecuteAndLog("vmstat",tname,fp,0,NULL,NULL);
	ExecuteAndLog("ps aux",tname,fp,0,NULL,NULL);
	ExecuteAndLog("users",tname,fp,0,NULL,NULL);

	ExecuteAndLog("lsof | wc -l",tname,fp,0,NULL,NULL);
#ifdef _RHEL_AS4_
	ExecuteAndLog("cat /proc/loadavg",tname,fp,0,NULL,NULL);
	ExecuteAndLog("cat /proc/diskstats",tname,fp,0,NULL,NULL);
	//in aix this value has been achieved in uptime syscal
#endif

	ExecuteAndLog("netstat -s",tname,fp,0,NULL,NULL);
	ExecuteAndLog("ps axl | sort -n -k6",tname,fp,0,NULL,NULL);
	ExecuteAndLog("ps axl | sort -n -k5",tname,fp,0,NULL,NULL);
	ExecuteAndLog("ps axu | sort -n -k3",tname,fp,0,NULL,NULL);
	ExecuteAndLog("ps axu | sort -n -k4",tname,fp,0,NULL,NULL);
	ExecuteAndLog("./ShmPrint",tname,fp,0,NULL,NULL);


	now = time(NULL);
	ts = localtime(&now);
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
	fprintf(fp,"************ End on %s ************\n",buf);

	fclose(fp);
	return 0;
}

int AppendTempToLog(FILE *fpLog,FILE *fpTemp,int nLogType,char *szSrc,char *szDst)
{
	int nSize=0;
	int j;
	char cTemp;
	if(fpTemp)
	{
		nSize=FileSize(fpTemp);
		if(!nSize)
		{
			fprintf(fpLog,"****>>\tNo Log Found");
		}
		while(nSize--)
		{
			cTemp=getc(fpTemp);
			if((unsigned char)cTemp==0xff)
			{
				j = 0;
				continue;
			}
			putc((int)cTemp,fpLog);
			if(cTemp=='\n' && (nSize-1))
			{

			}
		}

	}
	return (int)fpTemp;
}
int AppendFileToLog(FILE *fpLog,char *fname,int nLogType,char *szSrc,char *szDst)
{
	FILE *fpTmp=(FILE *)fopen(fname,"r");
	int nRet;
	if(	nRet = AppendTempToLog(fpLog,fpTmp,nLogType,szSrc,szDst))
	{
		fclose(fpTmp);
	}
	return nRet;
}
int FileSize(FILE *FInFp)
{
	int nPos,nFSize;
	nPos=ftell(FInFp);
	fseek(FInFp,0L,SEEK_END);
	nFSize=ftell(FInFp);
	fseek(FInFp,nPos,SEEK_SET);
	return nFSize;
}
int ExecuteAndLog(char *systemcalled,char *tempfile,FILE *logptr,int logType, char *src,char *dst)
{
	char       strsystem[MAX_SYSTEMSIZE];
	fprintf(logptr,"\n>%s :\n",systemcalled);
	memset(strsystem,'0',MAX_SYSTEMSIZE);
	sprintf(strsystem,"%s>%s",systemcalled,tempfile);
	system(strsystem);
	return AppendFileToLog(logptr,tempfile,logType,src,dst);
}
