/****************************************/
/*                                      */
/*      echo-all.c                      */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      forking echo server             */
/*                                      */
/*      25th February 1996              */
/*                                      */
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <sys/types.h>
#include  <sys/time.h>
#include  <errno.h>
#include  <signal.h>
#include <unistd.h>

#include "sock.h"

#define PORTNO  6789          /*  default port and host  */

int   port = PORTNO;

char *rem_host;
int   rem_port;

#define BUFF_LEN 200
char appbuff[BUFF_LEN];
int appbuf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

char *progname;

void usage()
{
    fprintf( stderr, "usage: %s remote-host remote-port\n", progname );
    exit(1);
} 
int write_string(int fd, char *str)
{
    return write(fd,str,strlen(str));
}
#define BUFLEN 512
char *exec_fail_mess="exec failed\n";
int my_system (char *cmd, int clientsk)
{
	int status = 0;
	int pfd[2];
	int pfd2[2];
	pid_t pid;
	char buf[BUFLEN];
	char *p = buf;	
	int exit_val = 0;
	char *argv[4];
	pipe (pfd);
	pipe (pfd2);
	pid = fork ();
	if (pid == -1)
		return -1;
	else if (pid == 0)
	{
		close (pfd[0]);
		close (1);
		dup (pfd[1]);
		//dup2 (pfd[1], STDOUT_FILENO);

		close (pfd2[1]);
		close (0);
		dup (pfd2[0]);
		//dup2 (pfd2[0], STDIN_FILENO);

		close (2);
		dup (STDOUT_FILENO);
		argv [0] = "sh";
		argv [1] = "-c";
		argv [2] = cmd;
		//argv [2] = "ls";
		argv [3] = NULL;
		exit_val = execv ("/bin/sh", argv);
		exit (0);
	}
	else
	{
		close (pfd[1]);
		close (0);
		dup (pfd[0]);
		//dup2 (pfd[0], STDIN_FILENO);

		close (pfd2[0]);
		close (1);
		dup (pfd2[1]);	
		//dup2 (pfd2[1], STDOUT_FILENO); 
	
		while (read (pfd[0], p , 1))
		{
			if (*p != '\n')
			{
				p ++;
				if ((p - buf) > BUFLEN)
				{
					write_string (clientsk, buf);
					p = buf;
					memset (buf, 0, BUFLEN);
				}
				continue;
			}
			write_string (clientsk, buf);
			*p = 0;
			p = buf;
			memset (buf, 0, BUFLEN);

		}
		//write_string (clientsk, "\n");
		if (waitpid (pid, &status, 0) == -1)
			return -1;
		else if (WIFEXITED (status))
			return WEXITSTATUS (status);
	
		return -1;
	}
}
/* End of my_system */

void failmess(char *mess)
{
    fprintf( stderr, "%s: %s\n", progname, mess );
    exit(1);
}

#define EOF_CHAR  4     /*  ctrl-D  */
int do_parse(int fd)  /* doesn't return */
{ 
	char szExec [BUFF_LEN];
	for(;;) 
	{
		memset (appbuff, 0, BUFF_LEN);
		int n = read(fd,appbuff,appbuf_len);
		if ( n < 0 )  
		{
			write_string(1,"knife child: error on input\n");
			exit(1);
		}
        	if ( n == 0  || appbuff[0] == EOF_CHAR )
		{
			write_string(1,"knife child: client connection closed\n");
			exit(0);
		} 
		appbuff[n - 2] = 0;
		appbuff[n - 1] = 0; 
		memset (szExec, 0, BUFF_LEN);
		memcpy (szExec, appbuff, n); 
   		my_system(szExec, fd);
        }
}
int accept_one_client(int port_sk);
    /*  returns 0 - error otherwise child pid  */
int do_echo(int fd);  /* doesn't return */



int main(int argc, char **argv)
{
    int    port_sk; 
    char *errmess; 

    progname = argv[0];
    if (argc >= 2) 
    {
      if (!strncmp (argv[1], "-h", 2))
      {
          fprintf (stdout, "usage %s -h : this help\n"
          "or %s port_number to listen in a specified port\n"
          "by default the port is 6789\n", argv[0], argv[0]);
          exit (1);
      }
      port = atoi (argv[1]); 
    }
	close (2);
	dup (1);
    if ( parse_network_args( &argc, argv, NULL, &port, &errmess ) != 0 )
        failmess(errmess);

  /*  establish port  */
    port_sk = tcp_passive_open(port);
        /*  wait for client on local port         */
    if ( port_sk < 0 ) { perror("socket"); exit(1); }
    fprintf(stderr,"start up complete\n");
 
    while ( accept_one_client(port_sk) > 0 ) /* do nout */;
    close(port_sk);
    exit(0);
}

char *welcome_mess="connected to echo-all, the multi-client echo server\n";
char *fork_mess="echo-all: fork successful\n";

int accept_one_client(int port_sk)
{
    int  client_sk;
    int  child_pid;

  /*  wait for client to connect  */
    client_sk = tcp_accept(port_sk);
    if ( client_sk < 0 ) {  /* on failure   return 0  */
        perror("socket");
        }

    write_string(client_sk,welcome_mess);

    child_pid = fork();
    if ( child_pid == 0 ) {  /* child */
        close(port_sk);
        write_string(client_sk,fork_mess);
        do_parse(client_sk);  /* doesn't return */
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






