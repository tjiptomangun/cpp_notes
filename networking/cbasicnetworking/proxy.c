/****************************************/
/*                                      */
/*      proxy.c                         */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      proxy server to listen in       */
/*      on TCP/IP protocols             */
/*                                      */
/*      15th January 1996               */
/*                                      */
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <sys/types.h>
#include  <sys/time.h>
#include  <errno.h>
#include  <signal.h>

#include "fd.h"
#include "sock.h"
#include <unistd.h>
#include <parsecfg.h>

#define PORTNO  6789          /*  default port and host  */

int   port = PORTNO;

char rem_host[100];
int  rem_port;
int  stop_listening_on_package_number;
int  package_number;

#define BUFF_LEN 200
char buff[BUFF_LEN];
char big_buff[4*BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

char *progname;
PLIST plist;
void usage()
{
	fprintf( stderr, "usage: %s remote-host remote-port\n", progname );
	exit(1);
}

void failmess(char *mess)
{
	fprintf( stderr, "%s: %s\n", progname, mess );
	exit(1);
}

int do_proxy( int client_sk, int serv_sk );
   /*  N.B.  both the following depend on there     */
   /*        being a spare byte at the end of buff  */
void log_from_client( char *buff, int len );
void log_from_server( char *buff, int len );


int nSigChild = 0;
void SigChild ()
{
   int signalled;
   int n;
   pid_t nPid;
   while (1)
   {
      switch ((nPid = waitpid (0, NULL, WNOHANG)))
      {
            case 0:
              return;
            case -1:
              continue;
            default:
              ++nSigChild;
              return;
      }
   }
}

int init_cfg(char *confname) {
	FILE *fp = fopen (confname, "r"); 
	#ifndef TMP_LEN 
	#define TMP_LEN	100 
	#endif
	int nret = 0;
	char tmp[TMP_LEN];
	if (!fp){
		fprintf (stdout, "configuration not exists : %s\n", confname);
		fprintf (stdout, "unable to find value : listening_port using default 6789\n");
		fprintf (stdout, "unable to find value : remote_host using default 127.0.0.1\n");  
		strcpy(rem_host, "127.0.0.1");
		fprintf (stdout, "unable to find value : remote_port using default 6789\n");
		rem_port = 6789;
		nret = -1;
		goto exit_code;
		
	}
	if (proc_cfg_file(fp, &plist)){
		fprintf (stdout, "unable to process config file : %s\n", confname);
		fprintf (stdout, "unable to find value : listening_port using default 6789\n");
		fprintf (stdout, "unable to find value : remote_host using default 127.0.0.1\n");
		nret = -1;
		goto exit_code;
		
	}
	if (get_cfg_val (plist, "listening_port", tmp, TMP_LEN - 1)){
		fprintf (stdout, "unable to find value : listening_port using default 6789\n");
	}
	else{
		port = atoi(tmp);
		fprintf (stdout, "listening_port %s    %d\n", tmp, port);
	}
	//memset (tmp, 0, TMP_LEN);
	//strcpy(rem_host, "10.100.102.67");

	if (get_cfg_val (plist, "remote_host", tmp, TMP_LEN - 1)){
		fprintf (stdout, "unable to find value : remote_host using default 127.0.0.1\n");
		strcpy(rem_host, "127.0.0.1");
	}
	else{
		strcpy(rem_host, tmp);
	}

	fprintf (stdout, "remote_host %s\n", rem_host);
	
	if (get_cfg_val (plist, "remote_port", tmp, TMP_LEN - 1)){
		fprintf(stdout, "unable to find value : remote_port using default 6789\n");
		rem_port = 6789;
	}
	else{
		rem_port = atoi(tmp);
	}

	if (get_cfg_val (plist, "stop_listening_on_package_number", tmp, TMP_LEN - 1)){
		fprintf(stdout, "unable to find value : stop_listening_on_package_number all package will be forwarded\n");
	}
	else{
		stop_listening_on_package_number= atoi(tmp);
		fprintf (stdout, "stop_listening_on_package_number %d\n", stop_listening_on_package_number);
	}
	
exit_code:
	if (fp){
		fclose (fp);
		plist->l_item.class.delete (&plist->l_item.class);
		
	}
	return nret; 
}
int accept_one_client(int port_sk)
{
	int  client_sk, serv_sk;
	int  child_pid;

  /*  wait for client to connect  */
	client_sk = tcp_accept(port_sk);
	if ( client_sk < 0 ) {  /* on failure   return 0  */
		perror("socket");
		}  
		
	child_pid = fork();
	if ( child_pid == 0 ) {  /* child */
		//close(port_sk);
		serv_sk = tcp_active_open(rem_host,rem_port);
			/*  now wait for remote server connection  */
			/*  returns negative result on failure     */
		if ( serv_sk < 0 ) {  /* on failure close down client connection  */
			perror("socket");
			close(client_sk);
            return -1;
		}
	
		fprintf(stderr,"connected to %s\n",rem_host);

		do_proxy(client_sk, serv_sk);  /* doesn't return */
		return 0;
	}
	else if ( child_pid > 0 ) {  /* parent */
		close(client_sk); 
		fprintf(stderr,"child process %d\n",child_pid);
		return child_pid;
	}
	else  {  /* error on fork */
		return 0;
	}
	
}
sigset_t  SigSet;
int main(int argc, char **argv)
{
	int    port_sk;
	int    ret;
	char   confname[100]; 
	int    rc;

	progname = argv[0];
	sprintf(confname, "%s.conf", progname);
	
	rc = init_cfg(confname);  
        if (signal (SIGCHLD, SigChild) == SIG_ERR)
        {
            printf("[ER] main signal");
            exit (250);
        }
        sigemptyset (&SigSet);
        sigaddset (&SigSet, SIGCHLD);

	fprintf(stderr,"proxy on local port#%d to remote-host %s on port#%d\n", port, rem_host, rem_port);

  /*  establish port  */ 
	port_sk = tcp_passive_open(port);
	/*  wait for client on local port         */
	if ( port_sk < 0 ) { 
		perror("socket"); 
		exit(1);
	}
	fprintf(stderr,"start up complete\n");
 
	/*  wait for client to connect  */
	while ( accept_one_client(port_sk) > 0 ) /* do nout */;
			close(port_sk);
	
	

	exit(ret);
}

int do_proxy( int client_sk, int serv_sk )
{
	fd_set     read_fds;
	fd_set     write_fds;
	fd_set     ex_fds;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&ex_fds);
	FD_SET(client_sk,&read_fds);
	FD_SET(serv_sk ,&read_fds);

	package_number = 0;
	for(;;) {
		int      num;
		fd_set   read_copy  =  read_fds;
		fd_set   write_copy =  write_fds;
		fd_set   ex_copy    =  ex_fds;
		static struct timeval timeout = {0,0};

		if (stop_listening_on_package_number && stop_listening_on_package_number < package_number){
			sleep(10);
			continue;
			
		}
			
		num = select(MAX_FD, &read_copy, &write_copy, &ex_copy, &timeout);
		if (num < 0 ) {
			if ( errno != EINTR ) {   /*  stopped by signal  */
				perror("select");
				continue;
				}
			perror("EINTR");
			return 1;
			}
		if ( num == 0 ) continue;   /*  timeout  */
		if ( FD_ISSET(client_sk,&read_copy) ) {
			int len = read( client_sk, buff, buf_len );
			if ( len <= 0 ) {  /*  error or close  */
				close(serv_sk);
				printf("**  client closed connection [%d]\n",len);
				return len;
				}
			else {
				write(serv_sk,buff,len);
				buff[len] = '\0';
			 /*   printf(">>  %s\n",buff);  */
				log_from_client( buff, len );
				}
			}
		if ( FD_ISSET(serv_sk ,&read_copy) ) {
			int len = read( serv_sk , buff, buf_len );
			if ( len <= 0 ) {  /*  error or close  */
				close(client_sk);
				printf("**  server closed connection [%d]\n",len);
				return len;
				}
			else {
				write(client_sk,buff,len);
				buff[len] = '\0';
		   /*      printf(">>  %s\n",buff);   */
				log_from_server( buff, len );
				package_number ++;
				}
			 }
		}
	return 0;
}

void log_general( char *prompt, char *cont, char *buff, int len )
	   /*  N.B.  buff must be at least len+1 bytes long  */
{
	int i, sol;
	char c = buff[len-1];
	for ( i=0, sol=0; i<len; i++ ) {
		int line_len, c=-1;
		while( i<len-1 && buff[i]!='\n' )  i++;
		line_len = i - sol + 1;
		if (i == len-1 && c != '\n' ) i++;
		buff[i] = 0;
		printf("%s%s  %d\n",prompt,&buff[sol],line_len);
		sol = i+1;
		}
	if ( c != '\n' ) puts(cont);
}

void  log_from_client( char *buff, int len )
{
	log_general( ">>  ", "... ", buff, len );
}


void  log_from_server( char *buff, int len )
{
	log_general( "<<  ", "... ", buff, len );
}


