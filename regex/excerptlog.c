#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
#include <time.h>
char *path_cat (const char *str1, char *str2);
#define BUFFER_SIZE 1028
#define MATCH_SIZE 4
typedef struct patt_presents
{
	int patt_type;

}PATT_PRESENTS;
int getLogDataLen(char *strIn,int nMaxLen)
{
#define BUFF_DATA_MAX_LEN 13
	char toConvert[BUFF_DATA_MAX_LEN+1];
	int i,StartFound=0;
	int j;
	for(i=0;i<BUFF_DATA_MAX_LEN+1;i++)
		toConvert[i]=0;
	for(i=j=0;i<nMaxLen;i++)
	{
		if(StartFound)
		{
			if(strIn[i]>='0' && strIn[i]<='9')
			{
				toConvert[j++]=strIn[i];
				if(j>=BUFF_DATA_MAX_LEN)
					break;
			}
			else
			{
				break;
			}
			
		}
		else
		{
			if(strIn[i]>='0' && strIn[i]<='9')
			{
				StartFound=1;
				toConvert[j++]=strIn[i];
			}
		}
	}
	if(StartFound)
		return atoi(toConvert);
	return -1;
}
int createLogStructTime(char *szfname,time_t nowtime,struct tm *structOutTime)
{
	int fnameLen = strlen(szfname), fileMon;
	char temp[4]={0,0,0,0};
	localtime_r(&nowtime,structOutTime);
	temp[0]=szfname[fnameLen-4];
	temp[1]=szfname[fnameLen-3];
	fileMon = atoi(temp);
	fileMon-=1;
	if(fileMon>structOutTime->tm_mon)
		structOutTime->tm_year-=1;
	structOutTime->tm_mon=fileMon;
	temp[0]=szfname[fnameLen-2];
	temp[1]=szfname[fnameLen-1];
	structOutTime->tm_mday=atoi(temp);
	structOutTime->tm_hour=structOutTime->tm_min=structOutTime->tm_sec=0;
	return 0;
}
int main (int argc, char * argv[])
{
// if no pattern then no process so user should fill all the flag, 
//except for the dir path, for which the default is ../log
	struct dirent *dp;
	regex_t regex_filepatt, regex_timepatt;
	regex_t regex_binpatt, regex_binreadwrite;
	regex_t regex_private;
	int reti;
	int ret;
	int patLen=0;
	FILE * fp,*fpLog;
	char strErr[256]={'\0'};
	char pattTimeFormat[20];
	char strLogTime[20];
	char strLine[BUFFER_SIZE];
	char prevLine[BUFFER_SIZE];
	const char *dir_path=argv[1];
	char *pat_date,pat_time[20],* elapsed;
	int nStartMillis=-1, nStartSecs=-1,nStartMins=-1,nStartHrs=-1;
	int nCurrHr=0,nCurrMin=0,nCurrSec=0;
	int nDelta=0;
	int nStartFound=0;
	long nStartFilePos;
	long nCurrFilePos;
	int nBinLen=0;
	int nBinRead=0;
	struct tm  logStrTime;
	time_t nowTime,logTime,endTime,currTime;
	DIR *dir;
	char *tmp;
	size_t numMatch=MATCH_SIZE;
	regmatch_t matchList[MATCH_SIZE];
	if(argc==2)
	{
		if(!strcmp(argv[1],"-h"))
		{
			fprintf(stdout,"excerptlog\n",argv[0]);
			fprintf(stdout,"NAME excerptlog\n");
			fprintf(stdout,"SYNOPSIS\n");
			fprintf(stdout,"\texcerptlog directory filePattern start_time duration\n");
			fprintf(stdout,"DESCRIPTION\n");
			fprintf(stdout,"\texcerpt xlink logs of filename format match regex filePattern,\n");
			fprintf(stdout,"\tof time start match format of start_time or something above it,\n");
			fprintf(stdout,"\tup to duration minutes,\n");
			exit(0);
		}
	}
	if(argc<5)
	{
		fprintf(stdout,"usage %s directory filePattern start duration\n",argv[0]);
		fprintf(stdout,"filePattern BRE (Basic Regular Expression)\n");
		fprintf(stdout,"start format HH[:MM][:SS]\n");
		fprintf(stdout,"duration in minutes\n");
		exit(0);
	}
	printf("argc :%d    ",argc);
	printf("path :%s    ",dir_path);
	printf("pat_file :%s    ",argv[2]);
	printf("pat_time :%s    ",argv[3]);
	printf("pat_elapsed :%s\n",argv[4]);
	fpLog = fopen("excerptlog.log","w");
	nDelta=atoi(argv[4]);
	sprintf(pat_time,"%s",argv[3]);
	patLen = strlen(pat_time);
	sprintf(pattTimeFormat,"^[[:space:]]{14}[0-9]{5}");
	reti = regcomp(&regex_binpatt,pattTimeFormat,REG_EXTENDED);
	if(reti)
	{
		fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
		fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);  
		fclose(fpLog);
		exit(1);
	}
	sprintf(pattTimeFormat,"^[0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{3}\\ {3,}[0-9A-F]{1,3}  XSock[R-W]");
    //sprintf(pattTimeFormat,"^[0-9][0-9]*");
	reti = regcomp(&regex_binreadwrite,pattTimeFormat,REG_EXTENDED);
	if(reti)
	{
		fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
		fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
		fclose(fpLog);
		exit(1);
	}
	sprintf(pattTimeFormat,"^[[:space:]]{18}0  Private[1-2]");
	reti = regcomp(&regex_private,pattTimeFormat,REG_EXTENDED);
	if(reti)
	{
		fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
		fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
		fclose(fpLog);
		exit(1);
	}
	//hh:mm:ss.millis
	sprintf(pattTimeFormat,"^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]\\.[0-9]*");// have millis
	reti = regcomp(&regex_filepatt,pattTimeFormat,REG_EXTENDED);
	if(reti)
	{
		fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
		fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
		fclose(fpLog);
		exit(1);
	}
	reti = regexec(&regex_filepatt,pat_time,0,NULL,0);
	if(!reti)
	{
		pat_time[2]=0;
		tmp=&pat_time[0];
		nStartHrs=atoi(tmp);
		tmp=&pat_time[3];
		pat_time[5]=0;
		nStartMins=atoi(tmp);
		tmp=&pat_time[6];
		pat_time[8]=0;
		nStartSecs=atoi(tmp);
		tmp=&pat_time[9];
		nStartMillis=atoi(tmp);
	}
	else
	{
		//hh:mm:ss
		sprintf(pattTimeFormat,"^[0-9]{2}:[0-9]{2}:[0-9]{2}");// have secs
		reti = regcomp(&regex_filepatt,pattTimeFormat,REG_EXTENDED);
		if(reti)
		{
			fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
			fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
			fclose(fpLog);
			exit(1);
		}
		reti = regexec(&regex_filepatt,pat_time,0,NULL,0);
		if(!reti)
		{
			pat_time[2]=0;
			tmp=&pat_time[0];
			nStartHrs=atoi(tmp);
			tmp=&pat_time[3];
			pat_time[5]=0;
			nStartMins=atoi(tmp);
			tmp=&pat_time[6];
			pat_time[8]=0;
			nStartSecs=atoi(tmp);
		}
		else
		{
			
			//hh:mm
			sprintf(pattTimeFormat,"^[0-9]{2}:[0-9]{2}");// have min
			reti = regcomp(&regex_filepatt,pattTimeFormat,REG_EXTENDED);
			if(reti)
			{
				fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
				fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
				fclose(fpLog);
				exit(1);
			}
			reti = regexec(&regex_filepatt,pat_time,0,NULL,0);
			if(!reti)
			{
				pat_time[2]=0;
				tmp=&pat_time[0];
				nStartHrs=atoi(tmp);
				tmp=&pat_time[3];
				pat_time[5]=0;
				nStartMins=atoi(tmp);
			}
			else
			{
				//hh
				sprintf(pattTimeFormat,"[0-9]{2}");// have hrs
				reti = regcomp(&regex_filepatt,pattTimeFormat,REG_EXTENDED);
				if(reti)
				{
					fprintf(stderr,"could not compile regex %s\n",pattTimeFormat);
					fprintf(fpLog,"could not compile regex %s\n",pattTimeFormat);
					fclose(fpLog);
					exit(1);
				}
				reti = regexec(&regex_filepatt,pat_time,0,NULL,0);
				if(!reti)
				{
					pat_time[2]=0;
					tmp=&pat_time[0];
					nStartHrs=atoi(tmp);
				}
				else
				{
					if(strcmp(pat_time,"*"))
					{
						fprintf(stderr,"time format [%s] does not match one "
						"of\n",pat_time);
						fprintf(stderr,"[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]*\n");
						fprintf(stderr,"[0-9]{2}:[0-9]{2}:[0-9]{2}\n");
						fprintf(stderr,"[0-9]{2}:[0-9]{2}\n");
						fprintf(stderr,"[0-9]{2}\n");
						fprintf(stderr,"*\n");
						fprintf(fpLog,"[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]*\n");
						fprintf(fpLog,"[0-9]{2}:[0-9]{2}:[0-9]{2}\n");
						fprintf(fpLog,"[0-9]{2}:[0-9]{2}\n");
						fprintf(fpLog,"[0-9]{2}\n");
						fprintf(fpLog,"*\n");
						fclose(fpLog);
						exit(0);
					}
				}

			}
		}
	}

	nowTime = time(NULL);
/*
	localtime_r(&nowTime,&logStrTime);
	pat_time[0]=pat_time[1]=0;
	memcpy(pat_time,&argv[2][0],2);
	if current month < than log month, then the log is last year log
	if(logStrTime.tm_mon < (atoi(pat_time)-1))
	{
		logStrTime.tm_year-=1;
	}
	logStrTime.tm_mon=atoi(pat_time)-1;
	memcpy(pat_time,&argv[2][2],2);
	logStrTime.tm_mday=atoi(pat_time);
	logStrTime.tm_hour=logStrTime.tm_min=logStrTime.tm_sec=0;
*/

	sprintf(pattTimeFormat,"^[0-9]{2}:[0-9]{2}:[0-9]{2}");    // have secs
	//sprintf(pattTimeFormat,"[0-9][0-9]:[0-9][0-9]");          // have mins
	//sprintf(pattTimeFormat,"[0-9][0-9]");                // have hrs
	reti = regcomp(&regex_timepatt,pattTimeFormat,REG_EXTENDED);
	if(reti){fprintf(stderr,"could not compile regex\n"); exit(1);}
	ret=access(dir_path,F_OK);
	if(ret!=0)
	{
		perror(strErr);
		printf("%s\n",strErr);
		exit(0);
	}
	dir = opendir(dir_path);
	while ((dp=readdir(dir)) != NULL) 
	{
		//logStrTime.tm_hour=logStrTime.tm_min=logStrTime.tm_sec=0;
		tmp = path_cat(dir_path, dp->d_name);
		reti = regcomp(&regex_filepatt,argv[2],REG_EXTENDED);
		if(reti){fprintf(stderr,"could not compile regex\n"); exit(1);}
		reti = regexec(&regex_filepatt,tmp,0,NULL,0);
		if(!reti)
		{
			printf("Match file %s\n", tmp);
			fp = fopen(tmp,"r");
			if(!fp)
			{
				fprintf(stderr,"error opening file %s\n",tmp);
				continue;
			}
			else
			{
				if(nStartHrs==-1)/*---   patt = "*"   ---*/
				{//match from start of file assumed time is 00:00 localtime
					//centinels are next minutes, nexthour, end of file
					fputs (tmp, stdout );
					createLogStructTime(tmp,nowTime,&logStrTime);
					logTime = mktime(&logStrTime);
					endTime = logTime+(nDelta*60);
					nStartFound=0;
					while(fgets ( strLine, BUFFER_SIZE, fp ))
					{
						nCurrFilePos = ftell(fp);
						if(strLine[0]==' ')
						{
							if(nStartFound)
							{
								if(REG_NOMATCH == (reti = regexec(&regex_binpatt,strLine,numMatch,matchList,0)))
								{
									fputs (strLine, stdout ); /* write the line */
								}
								else
								{//match binary patt. now does it a write/read xsock
									if(REG_NOMATCH == (reti = regexec(&regex_binreadwrite,prevLine,numMatch,matchList,0))
									   && REG_NOMATCH == (reti = regexec(&regex_private,prevLine,numMatch,matchList,0)))
									{
										fputs (strLine, stdout ); /* write the line */
									}
									else
									{//now it is confirmed binary, write binary
											strLine[21]=0;
											fputs (strLine, stdout ); /* write the line */
											nStartFilePos+=21;
											fseek(fp,nStartFilePos, SEEK_SET);
											nBinLen = getLogDataLen(strLine,BUFFER_SIZE);
											while(nBinLen>0)
											{
												nBinRead = fread(strLine,1,nBinLen>BUFFER_SIZE?BUFFER_SIZE:nBinLen,fp);
												fwrite(strLine,nBinRead,1,stdout);
												nBinLen-= nBinRead;
											}
											nCurrFilePos = ftell(fp);
									}
								}
							}

						}
						else if(strLine[0]<'0' || strLine[0]>'9')
						{
							fputs (strLine, stdout ); /* write the line */
						}
						else
						{
							if(REG_NOMATCH == (reti = regexec(&regex_timepatt,strLine,numMatch,matchList,0)))
							{
								fputs (strLine, stdout ); /* write the line */
							}
							else
							{//copy HH:MM:SS
								strncpy(strLogTime,strLine,8);
								nCurrSec = atoi(&strLogTime[6]);
								strLogTime[5]=0;
								nCurrMin = atoi(&strLogTime[3]);
								strLogTime[2]=0;
								nCurrHr = atoi(strLogTime);
								logStrTime.tm_hour = nCurrHr;
								logStrTime.tm_min = nCurrMin;
								logStrTime.tm_sec = nCurrSec;
								currTime = mktime(&logStrTime);
								if(currTime >endTime)
								{
									break;
								}
								else
								{
									fputs (strLine, stdout ); /* write the line */
								}
							}
						}
						memcpy(prevLine,strLine,BUFFER_SIZE);
						nStartFilePos = nCurrFilePos;
					}
				}
				else if(nStartMins==-1)
				{
					createLogStructTime(tmp,nowTime,&logStrTime);
					logStrTime.tm_hour=	nStartHrs;
					logTime = mktime(&logStrTime);
					endTime = logTime+(nDelta*60);
					nStartFound=0;
					nStartFilePos = ftell(fp);
					memset(prevLine,0,BUFFER_SIZE);
					while(fgets ( strLine, BUFFER_SIZE, fp ))
					{
						nCurrFilePos = ftell(fp);
						if(strLine[0]==' ')
						{
							if(nStartFound)
							{
								if(REG_NOMATCH == (reti = regexec(&regex_binpatt,strLine,numMatch,matchList,0)))
								{
									fputs (strLine, stdout ); /* write the line */
								}
								else
								{//match binary patt. now does it a write/read xsock
									if(REG_NOMATCH == (reti = regexec(&regex_binreadwrite,prevLine,numMatch,matchList,0))
									   && REG_NOMATCH == (reti = regexec(&regex_private,prevLine,numMatch,matchList,0)))
									{
										fputs (strLine, stdout ); /* write the line */
									}
									else
									{//now it is confirmed binary, write binary
											strLine[21]=0;
											fputs (strLine, stdout ); /* write the line */
											nStartFilePos+=21;
											fseek(fp,nStartFilePos, SEEK_SET);
											nBinLen = getLogDataLen(strLine,BUFFER_SIZE);
											while(nBinLen>0)
											{
												nBinRead = fread(strLine,1,nBinLen>BUFFER_SIZE?BUFFER_SIZE:nBinLen,fp);
												fwrite(strLine,nBinRead,1,stdout);
												nBinLen-= nBinRead;
											}
											nCurrFilePos = ftell(fp);
									}
								}
							}
						}
						else if(strLine[0]<'0' || strLine[0]>'9')
						{
							if(nStartFound)
								fputs (strLine, stdout ); /* write the line */
						}
						else
						{
							if(REG_NOMATCH == (reti = regexec(&regex_timepatt,strLine,numMatch,matchList,0)))
							{
								if(nStartFound)
									fputs (strLine, stdout ); /* write the line */
							}
							else
							{//copy HH:MM:SS
								strncpy(strLogTime,strLine,8);
								nCurrSec = atoi(&strLogTime[6]);
								strLogTime[5]=0;
								nCurrMin = atoi(&strLogTime[3]);
								strLogTime[2]=0;
								nCurrHr = atoi(strLogTime);
								logStrTime.tm_hour = nCurrHr;
								logStrTime.tm_min = nCurrMin;
								logStrTime.tm_sec = nCurrSec;
								currTime = mktime(&logStrTime);
								if(currTime <logTime)
								{
									fprintf(fpLog,"currTime <logTime; continue\n");
									continue;
								}
								else if(currTime >endTime)
								{
									fprintf(fpLog,"currTime >endTime; break\n");
									nStartFound=0;
									break;
								}
								else
								{
									nStartFound=1;
									fputs (strLine, stdout ); /* write the line */
								}
							}
						}
						memcpy(prevLine,strLine,BUFFER_SIZE);
						nStartFilePos = nCurrFilePos;
					}
				}
				else if(nStartSecs==-1)
				{
					createLogStructTime(tmp,nowTime,&logStrTime);
					logStrTime.tm_hour=	nStartHrs;
					logStrTime.tm_min=	nStartMins;
					logTime = mktime(&logStrTime);
					endTime = logTime+(nDelta*60);
					nStartFound=0;
					while(fgets ( strLine, BUFFER_SIZE, fp ))
					{
						nCurrFilePos = ftell(fp);
						if(strLine[0]==' ')
						{
							if(nStartFound)
							{
									if(REG_NOMATCH == (reti = regexec(&regex_binreadwrite,prevLine,numMatch,matchList,0))
									   && REG_NOMATCH == (reti = regexec(&regex_private,prevLine,numMatch,matchList,0)))
									{
										fputs (strLine, stdout ); /* write the line */
									}
									else
									{//now it is confirmed binary, write binary
											strLine[21]=0;
											fputs (strLine, stdout ); /* write the line */
											nStartFilePos+=21;
											fseek(fp,nStartFilePos, SEEK_SET);
											nBinLen = getLogDataLen(strLine,BUFFER_SIZE);
											while(nBinLen>0)
											{
												nBinRead = fread(strLine,1,nBinLen>BUFFER_SIZE?BUFFER_SIZE:nBinLen,fp);
												fwrite(strLine,nBinRead,1,stdout);
												nBinLen-= nBinRead;
											}
											nCurrFilePos = ftell(fp);
									}
								//fputs (strLine, stdout ); /* write the line */
							}

						}
						else if(strLine[0]<'0' || strLine[0]>'9')
						{
							if(nStartFound)
								fputs (strLine, stdout ); /* write the line */
						}
						else
						{
							if(REG_NOMATCH == (reti = regexec(&regex_timepatt,strLine,numMatch,matchList,0)))
							{
								if(nStartFound)
									fputs (strLine, stdout ); /* write the line */
							}
							else
							{//copy HH:MM:SS
								strncpy(strLogTime,strLine,8);
								nCurrSec = atoi(&strLogTime[6]);
								strLogTime[5]=0;
								nCurrMin = atoi(&strLogTime[3]);
								strLogTime[2]=0;
								nCurrHr = atoi(strLogTime);
								logStrTime.tm_hour = nCurrHr;
								logStrTime.tm_min = nCurrMin;
								logStrTime.tm_sec = nCurrSec;
								currTime = mktime(&logStrTime);
								if(currTime <logTime)
								{
									continue;
								}
								else if(currTime >endTime)
								{
									nStartFound=0;
									break;
								}
								else
								{
									nStartFound=1;
									fputs (strLine, stdout ); /* write the line */
								}
							}
						}
						memcpy(prevLine,strLine,BUFFER_SIZE);
						nStartFilePos = nCurrFilePos;
					}
				}
				else
				{
					createLogStructTime(tmp,nowTime,&logStrTime);
					logStrTime.tm_hour=	nStartHrs;
					logStrTime.tm_min=	nStartMins;
					logStrTime.tm_sec=	nStartSecs;
					logTime = mktime(&logStrTime);
					endTime = logTime+(nDelta*60);
					nStartFound=0;
					while(fgets ( strLine, BUFFER_SIZE, fp ))
					{
						nCurrFilePos = ftell(fp);
						if(strLine[0]==' ')
						{
							if(nStartFound)
							{
									if(REG_NOMATCH == (reti = regexec(&regex_binreadwrite,prevLine,numMatch,matchList,0))
									   && REG_NOMATCH == (reti = regexec(&regex_private,prevLine,numMatch,matchList,0)))
									{
										fputs (strLine, stdout ); /* write the line */
									}
									else
									{//now it is confirmed binary, write binary
											strLine[21]=0;
											fputs (strLine, stdout ); /* write the line */
											nStartFilePos+=21;
											fseek(fp,nStartFilePos, SEEK_SET);
											nBinLen = getLogDataLen(strLine,BUFFER_SIZE);
											while(nBinLen>0)
											{
												nBinRead = fread(strLine,1,nBinLen>BUFFER_SIZE?BUFFER_SIZE:nBinLen,fp);
												fwrite(strLine,nBinRead,1,stdout);
												nBinLen-= nBinRead;
											}
											nCurrFilePos = ftell(fp);
									}
							}
						}
						else if(strLine[0]<'0' || strLine[0]>'9')
						{
							if(nStartFound)
								fputs (strLine, stdout ); /* write the line */
						}
						else
						{
							if(REG_NOMATCH == (reti = regexec(&regex_timepatt,strLine,numMatch,matchList,0)))
							{
								if(nStartFound)
									fputs (strLine, stdout ); /* write the line */
							}
							else
							{//copy HH:MM:SS
								strncpy(strLogTime,strLine,8);
								nCurrSec = atoi(&strLogTime[6]);
								strLogTime[5]=0;
								nCurrMin = atoi(&strLogTime[3]);
								strLogTime[2]=0;
								nCurrHr = atoi(strLogTime);
								logStrTime.tm_hour = nCurrHr;
								logStrTime.tm_min = nCurrMin;
								logStrTime.tm_sec = nCurrSec;
								currTime = mktime(&logStrTime);
								if(currTime <logTime)
								{
									continue;
								}
								else if(currTime >endTime)
								{
									nStartFound=0;
									break;
								}
								else
								{
									nStartFound=1;
									fputs (strLine, stdout ); /* write the line */
								}
							}
						}
						memcpy(prevLine,strLine,BUFFER_SIZE);
						nStartFilePos = nCurrFilePos;
					}
				}
			}
			fclose(fp);
		}
		free(tmp);
		tmp=NULL;
	}
	closedir(dir);
	fclose(fpLog);
	return 0;
}
/*
 this function concatenates a directory with a filename
 adding '/' if the directory does not ended with one
*/
char *path_cat (const char *parent, char *child) {
	int i,j;
	size_t str1_len = strlen(parent);
	size_t str2_len = strlen(child);
	char *result;
	result = (char *)malloc((str1_len+str2_len+2)*sizeof (char));
	memset(result ,0,(str1_len+str2_len+2)*sizeof (char));
	if(parent[str1_len - 1]=='/')
	{
		strncpy (result,parent,str1_len-1);
	}
	else
	{
		strcpy (result,parent);
	} 
	strcat(result,"/");
	
        str1_len = i=strlen(result);
	for(j=0; j<str2_len;i++, j++) {
		result[i]=child[j];
	}
	result[str1_len+str2_len]='\0';
	return result;
}
