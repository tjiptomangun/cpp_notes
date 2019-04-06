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
char buff[BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

char *progname;

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

int accept_one_client(int port_sk);
    /*  returns 0 - error otherwise child pid  */
int do_echo(int fd);  /* doesn't return */

int write_string(int fd, char *str)
{
    return write(fd,str,strlen(str));
}


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
        do_echo(client_sk);  /* doesn't return */
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

char *exec_fail_mess="exec failed\n";
#define EOF_CHAR  4     /*  ctrl-D  */

char *argv[2] = { "sh", NULL };

int do_echo(int fd)  /* doesn't return */
{
    for(;;) {
        int n = read(fd,buff,buf_len);
        if ( n < 0 )  {
            write_string(1,"knife child: error on input\n");
            exit(1);
            }
        if ( n == 0  || buff[0] == EOF_CHAR )  {
            write_string(1,"knife child: client connection closed\n");
            exit(0);
            }
        write(fd,buff,n);
        }
}


