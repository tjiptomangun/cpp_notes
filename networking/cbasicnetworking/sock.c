/****************************************/
/*                                      */
/*      sock.c                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      package up socket garbage       */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#include "sock.h" 

int  parse_network_args( argc, argv, hostname, portno, errmess )
    int *argc;
    char **argv;
    char **hostname;
    int *portno;
    char **errmess;
{
    char **from = &argv[1],
         **to   = &argv[1];
    while ( *from ) {
        if ( strcmp(*from,"-host") == 0 ) {
            from ++;
            if ( hostname == NULL ) {
                *errmess = "cannot set host name";
                return 1;
                }
            if ( !*from ) {
                *errmess = "-host requires host name";
                return 1;
                }
            *hostname = *from;
            from ++;
            *argc -= 2;;
            }
        else  if ( strcmp(*from,"-port") == 0 ) {
            from ++;
            if ( portno == NULL ) {
                *errmess = "cannot set port number";
                return 1;
                }
            if ( !*from || !isdigit(**from) ) {
                *errmess = "-port requires port number";
                return 1;
                }
            *portno = atoi(*from);
            from ++;
            *argc -= 2;;
            }
        else {
            if ( to != from ) *to = *from;
            from ++;
            to ++;
            }
        }
    if ( to != from ) *to = NULL;
    return 0;
}


int  tcp_passive_open(portno)
    int    portno;
{
    int     sd, code;
    struct  sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = 0;    /*  0.0.0.0  ==  this host  */
    bzero(bind_addr.sin_zero, 8);
    bind_addr.sin_port = htons(portno);
    sd = socket(AF_INET, SOCK_STREAM,0);
    if ( sd < 0 ) return sd;
    code = bind(sd, (struct sockaddr *)&bind_addr, sizeof(bind_addr) );
    if ( code < 0 ) { close(sd); return code; }
    code = listen(sd, 1);
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

int  tcp_active_open(hostname,portno)
    char * hostname;
    int    portno;
{
    int     sd, code;
    struct  sockaddr_in bind_addr;
    struct hostent *host;

    host = gethostbyname(hostname);
    if (host == NULL ) return -1;
    bind_addr.sin_family = PF_INET;
    bind_addr.sin_addr = *((struct in_addr *) (host->h_addr));
    bind_addr.sin_port = htons(portno);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sd < 0 ) return sd;
    code = connect(sd, (struct sockaddr *) &bind_addr, sizeof(bind_addr) );
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

int  tcp_accept(sock)
    int sock;
{
    int     sd;
    struct  sockaddr bind_addr;
    int len=sizeof(bind_addr);
    sd = accept(sock, (struct sockaddr *) &bind_addr, (socklen_t *) &len);
    return sd;
}

