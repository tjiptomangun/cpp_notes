/****************************************/
/*                                      */
/*      simple-server.c                 */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      server for a simple 'talk'      */
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

main(int argc, char **argv)
{
    int    port_sk,  client_sk;
    int    len;
    char *errmess;

    progname = argv[0];

    if ( parse_network_args( &argc, argv, NULL, &port, &errmess ) != 0 )
        failmess(errmess);

  /*  establish port  */
    port_sk = tcp_passive_open(port);
        /*  local host for server                 */
        /*  returns negative result on failure    */
    if ( port_sk < 0 ) { perror("socket"); exit(1); }
    printf("start up complete\n");
 
  /*  wait for client to connect  */
    client_sk = tcp_accept(port_sk);

  /*  only want one client, so close port_sk  */
    close(port_sk);

  /*  talk to client  */
    for(;;) {
      /*  wait for clientÕs message  */
        len = read(client_sk,buff,buf_len); 
        if (len == 0) {
            printf("client finished the conversation\n");
            break;
            }
        buff[len] = '\0';
        printf("client says: %s\n",buff);
      /*  now itÕs our turn  */
        printf("speak: ");
        if ( gets(buff) == NULL ) {    /* user typed end of file  */
            close(client_sk);
            printf("bye bye\n");
            break;
            }
        write(client_sk,buff,strlen(buff));
    }
    exit(0);
}
