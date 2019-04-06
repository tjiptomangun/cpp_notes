/*http://www.tidytutorials.com/2010/06/linux-c-socket-example-with-client.html*/
/* CC -ggdb3 -Wall xshellsvr.c -o xshellsvr -lpthread  */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
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
char appname [BUFF_UNIT];
/*
 * Name  	: XLHashBangCheck
 * Desc		: Find a hashbang in a specified file
 * Input	
 * 	filename	: name of the file
 *	out		: buffer to store the hashbang commanf
 *	out_len		: output max len	

 */
int XLHashBangCheck( char* filename, char *out , int out_len)
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
		memset (out, 0, out_len );
		return FILE_NOTEXISTS;
	}

	if (fread (in_buff, 1, 1, instream )!= 1 )
	{
		fclose (instream );
		return FILE_HASHBANG_NONE;
	}
	c1 = in_buff[0];
	in_buff[0] = 0;
	if (fread (in_buff, 1, 1, instream )!= 1 )
	{
		fclose (instream );
		return FILE_HASHBANG_NONE;
	}
	c2 = in_buff[0];
	in_buff[0] = 0;
	if ( c1 == '#' && c2 == '!' )
		result = FILE_HASHBANG_EXISTS;

	while ((bytes_read = fread (&in_buff[0], 1, 1, instream )) == 1 )
	{
		if (in_buff[0] == 0x0d || in_buff[0] == 0x0a )
			break;
		out[curr_len] = in_buff[0];	
		curr_len +=1;
		if (curr_len >= out_len)
		{
			break ;
		}
	}
	printf ("\%s\n",out);
	fclose (instream);
	return result ;	
}

/*
 * Name 	: ExecSystem
 * Desc		: Exec and log a shell command. 
 * Input 	:
 *	process : name of the process
 *	daemonize: if true then append daemon sign (&) at the end of command.
 *	It implies that we will not wait for system to return.
 */
int ExecSystem(char * process, short daemonize )
{
	time_t now;
	struct tm tmnow;
	char logbuff[BUFF_UNIT];
	char systemparm[SYSTEMPARM_BUFF];
	char timeexec[BUFF_UNIT];

	memset ( logbuff, 0, BUFF_UNIT );
	memset ( systemparm, 0, SYSTEMPARM_BUFF );
	memset ( timeexec, 0, BUFF_UNIT );

	time ( &now );
	gmtime_r ( &now, &tmnow );
	
	sprintf ( logbuff , "../log/%s_%02d%02d", appname, 
		tmnow.tm_mon + 1, tmnow.tm_mday );
	sprintf (timeexec, "%02d%02d%02d%02d%02d%02d", tmnow.tm_year-100, 
		tmnow.tm_mon + 1, tmnow.tm_mday, tmnow.tm_hour, 
		tmnow.tm_min, tmnow.tm_sec);
	sprintf ( systemparm, "%s %s >>%s 2>&1", "echo ", timeexec , logbuff );
	system ( systemparm );
	memset ( systemparm, 0, SYSTEMPARM_BUFF );
	if (daemonize )
	{
		sprintf ( systemparm, "%s >>%s 2>&1 &", process , logbuff ); 
	}
	else
	{
		sprintf ( systemparm, "%s >>%s 2>&1 ", process , logbuff ); 
	}
	system ( systemparm );
	fprintf ( stdout , "executing %s\n" , systemparm );
	return 0;
}

/*
 * SendOptions
 * description : printing option routines to client socket
 * returns :
 * 	SEND_OK on success 
 * 	others problems
 */
int SendOptions ( int *csock , int state )
{
	char buffer[1024];
	int  buffer_len = 1024;
	int bytecount  = 0;
	memset ( buffer, 0, buffer_len );
	switch (state)
	{
	case  STATE_EXECSTART :
		strcat ( buffer, "Start Executed");
		break;
	case STATE_EXECSTOP :
		strcat ( buffer, "Stop  Executed");
		break;
	case STATE_EXECTERM:
		strcat ( buffer, "Terminate Executed");
		break;
	case STATE_EXECKILL :
		strcat ( buffer, "Kill Executed");
		break;	
	default :
		strcat ( buffer, "SERVER RESPONSE");
		strcat ( buffer, "\nOptions:\n1.Start X/Link\n2.Stop X/Link\n"); 
	} 
	if (( bytecount = send (*csock, buffer, strlen(buffer), 0)) == -1)
	{
		fprintf( stderr, "Error sending data %d\n", errno);
		return SEND_ERR;
	}
	printf("Send bytes %d\n", bytecount );
	return SEND_OK;
}


void *RequestHandler( void *p_rq )
{
	pid_t thrid = pthread_self();

	int rq = * (int * )p_rq;
	int hashbang = 0;

	char shell[ SHELL_BUFF ];
	char command[ COMMAND_BUFF];

	memset (shell, 0, SHELL_BUFF );

	switch (rq )
	{
	case RQ_XSTART :
		ExecSystem ("XLink", 1);
		break;

	case RQ_XSTOP :
		ExecSystem ("XLStop -f", 0);
		break;

	default:
		hashbang = XLHashBangCheck( "XLink", shell, SHELL_BUFF -1 ); 
		switch (hashbang )
		{
		case FILE_NOTEXISTS:
			rq = 0;
			break;	
		case FILE_HASHBANG_NONE:
			sprintf (shell, "sh -c" );
			break;
		case FILE_HASHBANG_EXISTS:
			break;
		default:
			rq = 0;
			break;
		} 
		memset (command , 0 , COMMAND_BUFF);
		switch (rq)
		{
		case RQ_XTERMSH:
			sprintf (command, "`ps ax|grep \"%s XLink\"|grep -v grep|sed -e 's/ *\\([^ ]*\\) *.*/kill -%d \\1/g'`", shell, SIGTERM);
			ExecSystem (command, 0);
			break;
		case RQ_XKILLSH:
			sprintf (command, "`ps ax|grep \"%s XLink\"|grep -v grep|sed -e 's/ *\\([^ ]*\\) *.*/kill -%d \\1/g'`", shell, SIGKILL);
			ExecSystem (command, 0);
			break;
		default :
			break;
		}
		break;	
	}
	printf("thread_id rqhandler  %d returns\n", thrid);
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
	int  count = 1;
	while ( 1 )
	{	
		memset ( buffer, 0, buffer_len );

		if ( state == STATE_SIGNON )
		{
			fprintf (stdout, "wait for data ...\n");
			if ( ( bytecount = recv ( *csock, buffer, buffer_len, 0)) == -1)
			{
				printf("received bytes %d\nReceived string\"%s\"\n", bytecount, buffer );
				fprintf ( stderr, "error receiving data %d\n", errno);
				perror ("recv");
				free (csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			if ( strcmp (buffer , "sign on" ) )/*not of our client*/
			{
				fprintf (stderr, "not of our client\n"  );
				fprintf (stderr, "%s\n", buffer);
				free(csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			}
			if ((count % 13) == 0)
			{
				sleep (2);
				printf ("wake\n");
			}
			if ( SendOptions ( csock, state ) )
			{
				free ( csock);
				printf("thread_id %d returns\n", thrid);
				return 0;
			} 
			if ((count % 17) == 0)
			{
				sleep (2);
				printf ("wake\n");
			}
			state = STATE_SIGNON ;
			count ++;
		}

	} 
	fprintf(stderr, "thread_id sockhandler %d returns\n", thrid);
	return 0;

}
/*
 *reference on htonl on loopback
 *1. http://stackoverflow.com/questions/6081892/does-sin-addr-s-addr-inaddr-any-need-htonl-at-all
 *2. inetin/in.h and winsock.h 
 */
/*#define INADDR_LOOPBACK     0x7f000001   127.0.0.1   */


int main (int argc, char **argv)
{
	int host_port = 1101;
	struct sockaddr_in my_addr;
	
	int hsock = 0;

	int *p_int;

	socklen_t addr_size = 0;
	int *csock;
	struct sockaddr_in sadr;
	pthread_t thread_id = 0;
	sprintf (appname , "%s", "xshellsvr" );
	
	hsock = socket (AF_INET, SOCK_STREAM, 0);
	if ( hsock == -1 )
	{
		fprintf (stderr, "error initializing socket %d\n", errno );
		exit (1);	
	}

	p_int = (int *) calloc ( 1, sizeof (int ));
	*p_int = 1;
	
	if ( ( setsockopt ( hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, sizeof( int )) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, sizeof( int )) == -1))
	{
		fprintf ( stderr, "error setting options %d\n", errno );
		free (p_int );
		exit (1);
	}
	
	free ( p_int);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(host_port);

	memset ( &(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);

	if ( bind (hsock , (struct sockaddr *)&my_addr, sizeof ( my_addr )) == -1)
	{
		fprintf ( stderr, "error binding socket %d err %d",host_port, errno );
		exit (1);
	}
	
	if ( listen( hsock, 10 ) == -1)
	{
		fprintf (stderr, "error listening %d\n", errno );
		exit (1);
	}
	//Now let us do the server stuff

	while ( 1 )
	{
		fprintf (stdout, "waiting for a connection\n");
		csock = (int *) calloc (1, sizeof (int ));
		if ( ( *csock = accept ( hsock , (struct sockaddr *) &sadr, &addr_size)) != -1 )
		{
			fprintf (stdout, "------------\nconnection from %s\n", 
				inet_ntoa(sadr.sin_addr));
			pthread_create ( &thread_id, 0, &SocketHandler, (void *)csock );
			pthread_detach ( thread_id );
		}
		else
		{
			fprintf (stderr , "error accepting %d\n", errno );
		}
		sleep ( 1 );
	}
}
