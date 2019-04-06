/****************************************/
/*                                      */
/*      simple-client.c                 */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      client for a simple 'talk'      */
/*      application with strict         */
/*      turn-taking                     */
/*                                      */
/*      15th January 1996               */
/*                                      */
/****************************************/

#include <stdio.h>
#include <string.h>
#include "sock.h"


#define PORTNO  6789          /*  default port and host  */
#define HOST    "zeus"

char *host = HOST;
int   port = PORTNO;

#define BUFF_LEN 200
char buff[BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

char *progname;

void failmess(char *mess)
{
    fprintf( stderr, "%s: %s\n", progname, mess );
    exit(1);
}

main( int argc, char**argv )
{
    int  serv_sk, len;
    char *errmess;

    progname = argv[0];

    if ( parse_network_args( &argc, argv, &host, &port, &errmess ) != 0 )
        failmess(errmess);

  /*  request connection to server  */
    serv_sk = tcp_active_open(host,port);
        /*  waits for server to accept             */
        /*  returns negative result on failure     */
        /*  host is server’s machine               */
    if ( serv_sk < 0 ) { perror("socket"); exit(1); }
    printf("You can send now\n");
 
/*  talk to server  */
    for(;;) {
      /*  our turn first  */
        printf("speak: ");
        if ( gets(buff) == NULL ) {    /* user typed end of file  */
            close(serv_sk);
            printf("bye bye\n");
            break;
            }
        write(serv_sk,buff,strlen(buff));

      /*  wait for server’s message  */
        len = read(serv_sk,buff,buf_len); 
        if (len == 0) {
            printf("server finished the conversation\n");
            break;
            }
        buff[len] = '\0';
        printf("server says: %s\n",buff);
    }
    exit(0);
}

