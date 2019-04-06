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
#include <string.h>
#include  <sys/types.h>
#include  <sys/time.h>
#include  <errno.h>
#include  <signal.h>

#include "fd.h"
#include "sock.h"

#define PORTNO  6789          /*  default port and host  */

int   port = PORTNO;

char *rem_host;
int   rem_port;

#define BUFF_LEN 200
char buff[BUFF_LEN];
char big_buff[4*BUFF_LEN];
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

int do_proxy( int client_sk, int serv_sk );
   /*  N.B.  both the following depend on there     */
   /*        being a spare byte at the end of buff  */
void log_from_client( char *buff, int len );
void log_from_server( char *buff, int len );


main(int argc, char **argv)
{
    int    port_sk,  client_sk, serv_sk;
    int    ret;
    char *errmess;

    progname = argv[0];

    if ( parse_network_args( &argc, argv, NULL, &port, &errmess ) != 0 )
        failmess(errmess);

    if ( argc != 3 ) usage();
    rem_host = argv[1];
    rem_port = atoi(argv[2]);

    fprintf(stderr,"proxy on local port#%d to remote-host %s on port#%d\n",
                                       port,           rem_host, rem_port);

  /*  establish port  */
    port_sk = tcp_passive_open(port);
        /*  wait for client on local port         */
    if ( port_sk < 0 ) { perror("socket"); exit(1); }
    fprintf(stderr,"start up complete\n");
 
  /*  wait for client to connect  */
    client_sk = tcp_accept(port_sk);

  /*  only want one client, so close port_sk  */
    close(port_sk);

    serv_sk = tcp_active_open(rem_host,rem_port);
        /*  now wait for remote server connection  */
        /*  returns negative result on failure     */
    if ( serv_sk < 0 ) {  /* on failure close down client connection  */
        perror("socket");
        close(client_sk);
        exit(1);
        }

    fprintf(stderr,"connected to %s\n",rem_host);

    ret = do_proxy( client_sk, serv_sk );

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

    for(;;) {
        int      num, len;
        fd_set   read_copy  =  read_fds;
        fd_set   write_copy =  write_fds;
        fd_set   ex_copy    =  ex_fds;
        static struct timeval timeout = {0,0};

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
        int line_len, c;
        while( i<len-1 && buff[i]!='\n' )  i++;
        line_len = i - sol + 1;
        if (i == len-1 && c != '\n' ) i++;
        buff[i] = 0;
        printf("%s%s\n",prompt,&buff[sol],line_len);
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


