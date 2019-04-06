/*******************************************/
/*                                         */
/*      mail-client.c                      */
/*                                         */
/*      Alan Dix                           */
/*                                         */
/*      simple mail client                 */
/*      takes from and to addresses on     */
/*      command line and sends using SMTP  */
/*                                         */
/*      22nd January 1996                  */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <string.h>
#include "sock.h"
#include "protocol.h"
#include "mail-helper.h"


#define PORTNO  25
#define HOST    "zeus"


char *host = HOST;
int   port = PORTNO;

#define BUFF_LEN 200
char buff[BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

char *progname;

void usage()
{
    fprintf( stderr, "usage: %s {tcp args} from to\n", progname );
    exit(1);
}

void failmess(char *mess)
{
    fprintf( stderr, "%s: %s\n", progname, mess );
    exit(1);
}

main( int argc, char**argv )
{
    int  server_fd, len, ret;
    char *errmess, *from, *to;

    progname = argv[0];

    if ( parse_network_args( &argc, argv, &host, &port, &errmess ) != 0 )
        failmess(errmess);

    if ( argc != 3 ) usage();
    from = argv[1];
    to   = argv[2];

    fprintf(stderr,"sending mail to host %s on port#%d\n",host,port);
    fprintf(stderr,"    from: %s\n",from);
    fprintf(stderr,"      to: %s\n",to);

  /*  request connection to server  */
    server_fd = tcp_active_open(host,port);
        /*  waits for server to accept             */
        /*  returns negative result on failure     */
        /*  host is server’s machine               */
    if ( server_fd < 0 ) { perror("socket"); exit(1); }
    fprintf(stderr,"connected to %s\n",host);
 
/*  talk to server  */
    ret = do_mail_init( server_fd );
    if ( ret == 0 )  ret = do_mail_from(server_fd , from);
    if ( ret == 0 )  ret = do_mail_to(server_fd , to);
    if ( ret == 0 )  ret = do_mail_data_fp(server_fd , stdin);
    if ( ret == 0 )  ret = do_mail_quit( server_fd );
    return ret;
}



