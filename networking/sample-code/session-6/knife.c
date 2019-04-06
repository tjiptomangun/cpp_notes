/****************************************/
/*                                      */
/*      knife.c                         */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      remote shell server             */
/*      !! very insecure !!             */
/*                                      */
/*      5th February 1996               */
/*                                      */
/****************************************/

#include <stdio.h>
#include <string.h>
#include  <sys/types.h>
#include  <sys/time.h>
#include  <errno.h>
#include  <signal.h>

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
int exec_a_shell(int fd);  /* doesn't return */

int write_string(int fd, char *str)
{
    return write(fd,str,strlen(str));
}


main(int argc, char **argv)
{
    int    port_sk;
    int    ret;
    char *errmess;

    progname = argv[0];

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

char *welcome_mess="connected to knife, the program that forks\n";
char *fork_mess="knife: fork successful\n";

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
        exec_a_shell(client_sk);  /* doesn't return */
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

int exec_a_shell(int fd)  /* doesn't return */
{
    int tty_fd;;
    dup2(fd,0);    /*  standard input  from fd  */
    dup2(fd,1);    /*  standard output to   fd  */
    dup2(fd,2);    /*  standard error  to   fd  */
    close(fd);
    execv("/bin/sh",argv);         /*  if successful doesn't return  */
    tty_fd = open("/dev/tty",1);   /*  N.B. stderr closed by dup2    */
    write(tty_fd,exec_fail_mess);
     _exit(1);
}


