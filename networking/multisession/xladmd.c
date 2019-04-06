/*http://www.tidytutorials.com/2010/06/linux-c-socket-example-with-client.html*/

/* CC -ggdb3 -Wall xshellsvr.c -o xshellsvr -lpthread  */
/* CC -ggdb4 -Wall xladmd.c -o xladmd -lpthread  */

/*
 * Name	       : xladmd
 * Description : XLink Stop/Start/Terminate Administrator.
 *
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

#include "parserclass.h"
#include "filecfgparser.h"

#define RQ_XSTART 1 
#define RQ_XSTOP  2
#define RQ_XTERMSH  3
#define RQ_XKILLSH 4

#define STATE_SIGNON 0
#define STATE_RQWAIT 1
#define STATE_EXECSTART STATE_RQWAIT + RQ_XSTART
#define STATE_EXECSTOP STATE_RQWAIT + RQ_XSTOP
#define STATE_EXECTERM STATE_RQWAIT + RQ_XTERMSH
#define STATE_EXECKILL STATE_RQWAIT + RQ_XKILLSH
#define STATE_DONE  STATE_RQ_WAIT + 100

#define SEND_OK 0
#define SEND_ERR 1
#define RECV_OK 0
#define RECV_ERR 1

#define FILE_NOTEXISTS -1
#define FILE_HASHBANG_NONE 0
#define FILE_HASHBANG_EXISTS 1


/*
 * thread system handler
 * try to do  a system()
 *  
 * 
*/

#define BUFF_UNIT 	40
#define SYSTEMPARM_BUFF (6 * BUFF_UNIT)
#define SHELL_BUFF 	(2 * BUFF_UNIT)
#define COMMAND_BUFF	(4 * BUFF_UNIT) 
char logname [256] = {0};


int nsigchild = 0; 
int host_port = 1101;

/**
 * NAME		: PrepareService
 * DESCRIPTION	: prepare this app for service
 *		  1. load config file
 *		  2. set up env variable
 *		  3. set log file
 **/
int PrepareService ()
{
	FILE *fp;
	PMINIPARSER pfile;
	char tokenlist [] = "[]#\n=";
	PLIST root = 0;
	PLIST curr = 0;
	PPROPERTY pprop = 0;
	char newenv [258]; 
	

	fp = fopen ("../dat/xladmd.ini", "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file ../dat/xladmd.ini\n");
		exit (1);
	} 

	pfile = newminiparser (fp, tokenlist, infile_parse);
	pfile->parse (pfile, &root);
	fclose (fp); 

	curr = (PLIST) root->takename (root, "environment"); 
	if (curr)
	{
		while ((pprop = (PPROPERTY) curr->take (curr)))
		{
			if (parse_env_str (pprop->value, newenv, 258))
			{
				setenv (pprop->l_item.class.name, newenv, 1); 
				pprop->l_item.class.preparedelete (&pprop->l_item.class);
			}
		} 
	} 
	curr->l_item.class.preparedelete (&curr->l_item.class);
	free (curr);

	
	curr = (PLIST) root->takename (root, "process"); 
	pprop = (PPROPERTY) curr->takename (curr, "LOG"); 
	if (pprop)
	{
		sprintf (logname, "%s", pprop->value);
	}
	else
	{
		fprintf (stderr, "please define \"LOG\" in xladmd.ini\n");
		exit (1);
	} 

	pprop->l_item.class.preparedelete (&pprop->l_item.class);
	free (pprop);

		
	pprop = (PPROPERTY) curr->takename (curr, "PORT"); 
	if (pprop)
	{
		if (strlen (pprop->value))
			host_port = atoi (pprop->value);

		pprop->l_item.class.preparedelete (&pprop->l_item.class);
		free (pprop);
	}
	

	root->l_item.class.preparedelete (&root->l_item.class);
	free (root); 

	return 0;
}/*PrepareService*/

int KillProcName (char *procname)
{
	char killerpatt [256] = {0};
	sprintf (killerpatt,"pids=`ps ax|sed -e '/ %s /!d;/grep/d;"
		"s/^ *\\([0-9]*\\) *.*/\\1/g'`; for pid in $pids ;"
		"do kill -9 $pid; done;", procname);
	system(killerpatt);
	return 0;	
}
/**
 * NAME		: KillSlayList
 * DESCRIPTION	: Open dat file, load to miniparser, kill slay list.
 * INPUT:
 *	FileName
 *		   
*/
int KillSlayList ()
{
	FILE *fp;
	PMINIPARSER pfile;
	char tokenlist [] = "[]#\n=";
	PLIST root = NULL;
	PLIST curr = NULL;
	PL_ITEM pitem = NULL;
	
	
	fp = fopen ("../dat/xladmd.ini", "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file ../dat/xladmd.ini\n");
		exit (1);
	} 

	pfile = newminiparser (fp, tokenlist, infile_parse);
	pfile->parse (pfile, &root);
	fclose (fp); 
	
	
	curr = (PLIST) root->takename (root, "slaylist"); 
	if (curr)
	{
		while ((pitem = curr->take (curr)))
		{
			KillProcName (pitem->class.name);
			pitem->class.delete (&pitem->class);
			
		} 
	} 
	curr->l_item.class.delete (&curr->l_item.class); 
	root->l_item.class.delete (&root->l_item.class);
	return 0;

}
/*
 * NAME		: Daemonize
 * DESCRIPTION	: daemonize this process
 */
int Daemonize()
{
	pid_t pid;
	int retval = -1;	
	int i; 
	long var_sysconf = 0;
	int fd = -1; 

	/* create new process */
	pid = fork ();
	if (pid == -1)
		exit (-1);
	else if (pid != 0)
		exit (EXIT_SUCCESS);
	
	/* create new session and process group */
	if (setsid() == -1)
		exit (-1); 

	/* close all open files, NR_OPEN and getdtablesize are neither posix standard */ 
	var_sysconf = sysconf (_SC_OPEN_MAX); 
	for (i = 0; i < 3; i++)
		close (i); 
	
	open ("/dev/null", O_RDWR); /* stdin */
	fd = open (logname, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | 
				S_IRUSR | S_IRGRP | S_IROTH);
	dup (fd); /* stdout */ 
	dup (1); /* stderr */ 
	retval = setvbuf (stdout, NULL, _IONBF, 0);
	retval = setvbuf (stderr, NULL, _IONBF, 0);

	return 0;

} /* Daemonize */
/***********************************************
 *check_pending : check for pending signal
 **********************************************/
void check_pending (int sig, char *signame, FILE *out)
{
	sigset_t sigset;
	if (sigpending (&sigset) != 0)
		fprintf (out, "sigpending () error\n");
	else if (sigismember (&sigset, sig))
		fprintf (out, "%s signal is pending\n", signame);
	else
		fprintf (out, "no %s signals are pending\n", signame);
	fprintf (out, "numsigchild =%d\n", nsigchild);
}
/**
 * Name        :sigchild
 * Description :signal handler for SIGCHLD.
 *              Wait for any child. XLink is given new session/group id.
 *              That is why first param of waitpid is -1.
 * 
 */ 
void sigchild()
{
	pid_t pid_child = 0; 
	while (1)
	{	
		switch ((pid_child = waitpid (-1, NULL, WNOHANG)))
		{
			case 0:
				return;
			case -1:
				continue;
			default:
				++nsigchild;
				return; 
		}
	} 
} 

/**
 * Name  	: XLHashBangCheck
 * Desc		: Find a hash bang (#!) in a specified file.
 *                Hash bang is valid if it appears as the first two 
 *                characters in a file.
 * Input	
 * 	filename	: name of the file
 *	out		: buffer to store the hashbang command.
 *	out_len		: output max len	
 */
int XLHashBangCheck(char* filename, char *out_buff , int out_len)
{
	FILE *instream = NULL;
	int bytes_read = 0;
	int curr_len = 0;
	int result = FILE_HASHBANG_NONE;
	char c1 = 0;
	char c2 = 0;
	char in_buff[2] = {0,0};

	instream = fopen (filename, "r");
	if (!instream)
	{
		memset (out_buff, 0, out_len);
		return FILE_NOTEXISTS;
	}

	if (fread (in_buff, 1, 1, instream)!= 1)
	{
		fclose (instream);
		return FILE_HASHBANG_NONE;
	}
	c1 = in_buff[0];
	in_buff[0] = 0;
	if (fread (in_buff, 1, 1, instream)!= 1)
	{
		fclose (instream);
		return FILE_HASHBANG_NONE;
	}
	c2 = in_buff[0];
	in_buff[0] = 0;
	if (c1 == '#' && c2 == '!')
		result = FILE_HASHBANG_EXISTS;

	while ((bytes_read = fread (&in_buff[0], 1, 1, instream)) == 1)
	{
		if (in_buff[0] == 0x0d || in_buff[0] == 0x0a)
			break;
		out_buff[curr_len] = in_buff[0];	
		curr_len +=1;
		if (curr_len >= out_len)
		{
			break ;
		}
	} 

	fclose (instream);

	return result ;	
}

/**
 * Name 	: ExecSystem
 * Desc		: Exec and log a shell command. 
 * Input 	:
 *	process : name of the process
 *	nowait  : if true then append daemon sign (&) at the end of command.
 *	It implies that we will not wait for system to return.
 * out		: output file
 */
int ExecSystem(char * process, short nowait, FILE *out)
{
	time_t now;
	struct tm tmnow;
	char logbuff[BUFF_UNIT];
	char systemparm[SYSTEMPARM_BUFF];
	char timeexec[BUFF_UNIT];

	memset (logbuff, 0, BUFF_UNIT);
	memset (systemparm, 0, SYSTEMPARM_BUFF);
	memset (timeexec, 0, BUFF_UNIT);

	time (&now);
	gmtime_r (&now, &tmnow);
	
	sprintf (logbuff , "%s", logname);
	sprintf (timeexec, "%02d %02d %02d %02d:%02d:%02d", tmnow.tm_year-100, 
		tmnow.tm_mon + 1, tmnow.tm_mday, tmnow.tm_hour, 
		tmnow.tm_min, tmnow.tm_sec);

	sprintf (systemparm, "%s %s >>%s 2>&1", "echo ", timeexec , logbuff);
	system (systemparm);

	memset (systemparm, 0, SYSTEMPARM_BUFF);
	if (nowait)
	{
		sprintf (systemparm, "%s >>%s 2>&1 &", process , logbuff); 
	}
	else
	{
		sprintf (systemparm, "%s >>%s 2>&1 ", process , logbuff); 
	}
	system (systemparm); 
	fprintf (out, "%s\n", systemparm); 
	return 0;
}

/**
 * Name	        : ExecSpawn
 * Description  : fork process. create a session for 
 *		  child process if create session is true.
 *                child process std output redirected to a log file.
 * Input         
 *          process_path   : process path
 *          argv           : c main arguments
 *          create_session : session flag 
 */ 
int ExecSpawn(char *process_path, char *argv[], 
		short int create_session)
{
	pid_t pid_active = 0 ;
	pid_t sess_id   = 0;	

	/* pid_t pid_active = 0 ; */

	int fdfile = -1;

	time_t now;
	struct tm tmnow;
	char logbuff[BUFF_UNIT];
	char systemparm[SYSTEMPARM_BUFF];
	char timeexec[BUFF_UNIT];

	if (access (process_path, R_OK| X_OK) != 0)
	{
		perror ("access");
		return -1;
	}
	switch ((pid_active = fork ()))
	{
	case 0:/*child*/
		if (create_session)
		{
			sess_id = setsid();
			if (sess_id < 0)
			{
				perror("setsid");
				return 0;
			}
		} 
		memset (logbuff, 0, BUFF_UNIT);
		memset (systemparm, 0, SYSTEMPARM_BUFF);
		memset (timeexec, 0, BUFF_UNIT);

		time (&now);
		gmtime_r (&now, &tmnow);
		
		sprintf (logbuff , "%s", logname);
		sprintf (timeexec, "%02d %02d %02d %02d:%02d:%02d", 
			tmnow.tm_year-100, 
			tmnow.tm_mon + 1, tmnow.tm_mday, tmnow.tm_hour, 
			tmnow.tm_min, tmnow.tm_sec);

		fdfile = open (logbuff, O_RDWR | O_CREAT| O_APPEND, 
						S_IRUSR | S_IWUSR);
		if (fdfile < 0)
		{
			perror("open failed\n");
			exit (1);
		}
		close (STDOUT_FILENO);
		dup (fdfile); 
		close (STDERR_FILENO);
		dup (fdfile); 
		close(fdfile);
		umask (027); 

		if (create_session)
			fprintf (stdout, "new sessid = %d\n", sess_id);
		execv (process_path, argv);
		fprintf (stdout ,"%s %s", timeexec, process_path);
		break;
	default:
		break;
	}
	return 0;
} 

/**
 * SendOptions
 * description : printing option routines to client socket
 * returns :
 * 	SEND_OK on success 
 * 	others problems
 */
int SendOptions (int *csock , int state)
{
	char buffer[1024];
	int  buffer_len = 1024;
	int bytecount  = 0;
	memset (buffer, 0, buffer_len);
	switch (state)
	{
	case  STATE_EXECSTART :
		strcat (buffer, "Start Executed");
		break;
	case STATE_EXECSTOP :
		strcat (buffer, "Stop  Executed");
		break;
	case STATE_EXECTERM:
		strcat (buffer, "Terminate Executed");
		break;
	case STATE_EXECKILL :
		strcat (buffer, "Kill Executed");
		break;	
	default :
		strcat (buffer, "SERVER RESPONSE");
		strcat (buffer, "\nOptions:\n1.Start X/Link\n2.Stop X/Link\n"); 
	} 
	if ((bytecount = send (*csock, buffer, strlen(buffer), 0)) == -1)
	{
		fprintf(stderr, "Error sending data %d\n", errno);
		return SEND_ERR;
	}
	printf("Send bytes %d\n", bytecount);
	return SEND_OK;
}
/**
 * NAME		: SignalSession
 * DESCRIPTION	: Sending signal to a session/process group.
 *		  It first signalling group id, then the group leader.
 * INPUT
 *	signal	: Signal number to send
 *	out	: where to output command
 *	
 *
 **/
int SignalSession (int signal, FILE *out)
{

	char command[ COMMAND_BUFF];
	char shell[ SHELL_BUFF ];
	int hashbang = 0; 

	memset (command, 0, COMMAND_BUFF);
	memset (shell, 0, SHELL_BUFF);
		hashbang = XLHashBangCheck("XLink", shell, SHELL_BUFF -1); 
		switch (hashbang)
		{
		case FILE_NOTEXISTS: 
			break;	
		case FILE_HASHBANG_NONE:
			sprintf (shell, "sh -c");
			break;
		case FILE_HASHBANG_EXISTS:
			break;
		default: 
			break;
		} 
		memset (command , 0 , COMMAND_BUFF);

	sprintf (command, "`ps ax|grep \"%s .*/*XLink\"|"
		"grep -v grep|sed -e 's/ *\\([^ ]*\\) *.*/kill -%d -\\1/g'`"
		, shell, signal);
	ExecSystem (command, 1, out); 

	memset (command, 0, COMMAND_BUFF);
	sprintf (command, "`ps ax|grep \"%s .*/*XLink\"|"
		"grep -v grep|sed -e 's/ *\\([^ ]*\\) *.*/kill -%d \\1/g'`"
		, shell, signal);
	ExecSystem (command, 1, out); 
	return 0;
}

/**
 * Name       : RequestHandler
 * Description: Thread to handle client request.
 *              For RQ_XTERM and RQ_XKILL we do shell script 
 *		but do not wait the result. 
 *              This is because we want to handle 
 *		sigchld that is the raise by
 *              the killing script.
 * Input      : Pointer to request value.
 */
void *RequestHandler(void *p_rq)
{
	char szXLink[] ="XLink";
	char szXLStop[] ="XLStop";
	char szXLStopParm[] ="-f";
	char *argv[7];

	pid_t thrid = pthread_self();

	int rq = * (int *)p_rq;

	switch (rq)
	{
	case RQ_XSTART :
		argv[0] = szXLink;
		argv[1] = 0;
		fprintf (stdout, "RQ_XSTART\n");
		ExecSpawn ("XLink", argv, 1);
		break;

	case RQ_XSTOP :
		argv[0] = szXLStop;
		argv[1] = szXLStopParm;
		argv[2] = 0;
		fprintf (stdout, "RQ_XSTOP\n");
		ExecSpawn ("XLStop", argv, 0);
		check_pending (SIGCHLD, "SIGCHLD",stdout);
		break;

	default:
		
		fprintf (stdout, "RQ_%d\n", rq);
		switch (rq)
		{
		case RQ_XTERMSH:
			SignalSession (SIGTERM, stdout);
			check_pending (SIGCHLD, "SIGHCHLD", stdout);
			break;
		case RQ_XKILLSH: 
			SignalSession (SIGKILL, stdout); 
			check_pending (SIGCHLD, "SIGHCHLD", stdout);
			KillSlayList ();
			break;
		default :
			break;
		}
		break;	
	}
	printf("thread_id rqhandler  %d returns\n", thrid);
	free (p_rq);
	return 0;
}

void *SocketHandler(void *lp)
{
	int *csock = (int *)lp;
	char buffer[1024];
	int  buffer_len = 1024;
	int  bytecount = 0;
	pid_t thrid = pthread_self();
	pthread_t thread_id = 0;
	int * rq = NULL;

	int  state = STATE_SIGNON;
	while (1)
	{
		memset (buffer, 0, buffer_len);
		if (state == STATE_SIGNON)
		{
			if ((bytecount = recv (*csock, buffer, buffer_len, 0)) == -1)
			{
				fprintf (stderr, "error receiving data %d\n", 
					errno);
				free (csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			if (bytecount == 0)
				break;
			if (strcmp (buffer , "sign on"))/*not of our client*/
			{
				fprintf (stderr, "err not of our client\n" );
				free(csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			if (SendOptions (csock, state))
			{
				free (csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			state = STATE_RQWAIT ;
		}
		else if (state == STATE_RQWAIT)
		{
			if ((bytecount = recv (*csock, buffer, buffer_len, 0)) == -1)
			{
				fprintf (stderr, "error receiving data %d\n", errno);
				free (csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			if (bytecount == 0)
				break;
			printf("received bytes %d\nReceived string\"%s\"\n", 
				bytecount, buffer);
			rq = (int *) calloc (1, sizeof (int));
			*rq = atoi(buffer);
			if (! (*rq == 1 || *rq == 2 || *rq == 3 || *rq == 4))
			{
				if (SendOptions (csock , state))
				{
					free (csock);
					printf("thread_id %d returns\n", thrid);
					return 0;
				}
				fprintf(stderr, "Error Rq\n");
				free (rq);
				free(csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			else 
			{
				state += *rq ;
				if (SendOptions (csock , state))
				{
					free (csock);
					printf("thread_id %d returns\n", thrid);
					return 0;
				}
			}
			pthread_create (&thread_id, 0, &RequestHandler, (void *)rq);
			pthread_detach (thread_id);
		}
		else
		{
			fprintf (stdout, "waiting dummies\n");
			if ((bytecount = recv (*csock, buffer, buffer_len, 0)) == -1)
			{
				
				fprintf (stderr, "error receiving data %d\n", errno);
				free (csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			fprintf (stdout, "waiting bycount = %d\n", bytecount);
			if (bytecount == 0)
				break;
		}
	}
	printf("thread_id sockhandler %d returns\n", thrid);
	return 0;

}
/*
 *reference on htonl on loopback
 *1. http://stackoverflow.com/questions/6081892/does-sin-addr-s-addr-inaddr-any-need-htonl-at-all
 *2. inetin/in.h and winsock.h 
 */
/*#define INADDR_LOOPBACK     0x7f000001   127.0.0.1   */

int usage()
{
	printf ("Usage: XALAdmAgent start|stop\n");
	exit (1);
}

char xladmdPidFile[] = "../dat/xladmd.pid";
int StartService ()
{ 
	char procaddrspace [128]; /* process address space */
	pid_t XLAdmPid  = 0;
	FILE *fppid;
	
	memset (procaddrspace , 0, 128);
	if ((fppid = fopen (xladmdPidFile,"r+w")))
	{
		fscanf (fppid, "%d", &XLAdmPid); 
#ifndef _RHEL4_AS_
		sprintf (procaddrspace, "/proc/%d/as", XLAdmPid);
#else
		sprintf (procaddrspace, "/proc/%d/maps", XLAdmPid);
#endif
		if (access (procaddrspace, F_OK) == 0)
		{
			fprintf (stderr, "Cannot start xladmd."
				" It is already running %d\n", XLAdmPid);
			fclose (fppid);
			exit (255);
		} 
		fclose (fppid);	
		remove (xladmdPidFile);
	}
	return 0;
}

int StopService ()
{ 
	char procaddrspace [128]; /* process address space */
	pid_t XLAdmPid  = 0;
	FILE *fppid; 
	FILE *out;

	if ((fppid = fopen (xladmdPidFile,"r")) == NULL)
	{
		perror (xladmdPidFile);
		return 0;
	} 
	memset (procaddrspace , 0, 128);
	if ((fppid = fopen (xladmdPidFile,"r+w")))
	{
		fscanf (fppid, "%d", &XLAdmPid); 
#ifndef _RHEL4_AS_
		sprintf (procaddrspace, "/proc/%d/as", XLAdmPid);
#else
		sprintf (procaddrspace, "/proc/%d/maps", XLAdmPid);
#endif
		if (access (procaddrspace, F_OK) == 0)
		{
			out = fopen (logname, "w+"); 
			SignalSession (SIGKILL, 0);
			check_pending (SIGCHLD, "SIGHCHLD", out);
			KillSlayList ();
			if (XLAdmPid > 0)
			{
				kill (-1*XLAdmPid, SIGKILL);
				kill (1*XLAdmPid, SIGKILL);
			} 
			if (out)
				fclose (out);
		} 
		fclose (fppid);	
		remove (xladmdPidFile);
	} 
	printf ("stopped\n"); 
	return 0;
}

/**
 * xladmd main
 * Listen in port and only accept from local address.
 * Create SocketHandler to handle session communication.
 */
int main (int argc, char **argv)
{
	/* signals */
	struct sigaction act; 
	FILE *fppid;
	

	struct sockaddr_in my_addr;
	
	int hsock = 0;

	int *p_int;

	if (argc < 2)
	{
		usage ();	
	}

	if ((strcasecmp (argv[1], "start") && strcasecmp (argv[1], "stop")
		&& strcasecmp (argv[1],"restart")))
	{
		usage ();
	}

	PrepareService(); 
	if (!strcasecmp (argv[1], "start"))
	{ 
		StartService ();
	}
	else if (!strcasecmp (argv[1], "stop"))
	{ 
		StopService ();
		exit (1);
	} 
	else if (!strcasecmp (argv[1], "restart"))
	{
		StopService ();
		StartService ();
	}

	socklen_t addr_size = 0;
	int *csock;
	struct sockaddr_in sadr;
	pthread_t thread_id = 0; 

	/*signal initialization*/
	memset (&act, 0, sizeof (act));
	act.sa_handler = sigchild;

	if (sigaction (SIGCHLD, &act, 0))
	{
		perror ("sigaction");
		exit (-1);
	}
 
	/*network stuffs */
	hsock = socket (AF_INET, SOCK_STREAM, 0);
	if (hsock == -1)
	{
		fprintf (stderr, "error initializing socket %d\n", errno);
		exit (1);	
	}

	p_int = (int *) calloc (1, sizeof (int));
	*p_int = 1;
	
	if ((setsockopt (hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, 
			sizeof(int)) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, 
			sizeof(int)) == -1))
	{
		fprintf (stderr, "error setting options %d\n", errno);
		free (p_int);
		exit (1);
	}
	
	free (p_int);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(host_port);

	memset (&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK); 

	if (bind (hsock , (struct sockaddr *)&my_addr, sizeof (my_addr)) == -1)
	{
		fprintf (stderr, "error binding socket %d err %d\n",host_port, errno);
		exit (1);
	} 
	if (listen(hsock, 10) == -1)
	{
		fprintf (stderr, "error listening %d\n", errno);
		exit (1);
	}

	fprintf (stdout,"started\n"); 

	Daemonize(); 
	//Now let us do the server stuff 

	if ((fppid = fopen (xladmdPidFile,"a+")) == NULL)
	{
		perror (xladmdPidFile);
		exit (1);
	}
	fprintf (fppid, "%d", getpid());
	fclose (fppid); 

	while (1)
	{
		fprintf (stdout, "waiting for a connection\n");
		csock = (int *) calloc (1, sizeof (int));
		if ((*csock = accept (hsock , (struct sockaddr *) &sadr, &addr_size)) != -1)
		{
			fprintf (stdout, "------------\nconnection from %s\n", 
				inet_ntoa(sadr.sin_addr));
			pthread_create (&thread_id, 0, &SocketHandler, (void *)csock);
			pthread_detach (thread_id);
		}
		else
		{
			fprintf (stderr , "error accepting %d\n", errno);
		}
	}
}
