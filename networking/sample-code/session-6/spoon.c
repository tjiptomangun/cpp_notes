/****************************************/
/*                                      */
/*      spoon.c                         */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      exec a shell                    */
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

    exec_a_shell(0);  /* doesn't return */

    exit(1);
}

char *welcome_mess="connected to knife, the program that forks\n";
char *fork_mess="knife: fork successful\n";


char *exec_fail_mess="exec failed\n";
#define EOF_CHAR  4     /*  ctrl-D  */

char *argv[2] = { "sh", NULL };

int exec_a_shell(int fd)  /* doesn't return */
{
    int tty_fd;;
/*    dup2(fd,0);  */
/*    dup2(fd,1);  */
/*    dup2(fd,2);  */
/*    close(fd);  */
    execv("/bin/sh",argv);
#if 0
    tty_fd = open("/dev/tty",1);
    for(;;) {
        int n = read(0,buff,buf_len);
        if ( n < 0 )  {
            write_string(tty_fd,"knife child: error on input\n");
            exit(1);
            }
        if ( n == 0  || buff[0] == EOF_CHAR )  {
            write_string(tty_fd,"knife child: client connection closed\n");
            exit(0);
            }
        write(1,buff,n);
        }
#endif
    write(tty_fd,exec_fail_mess);
     _exit(1);
}

