#include <stdio.h>
#include <string.h>
#include "mess.h"
#include "sock.h"
#include "inform.h"
#include "line_by_line.h"

char in_buff[256], out_buff[256], line_buff[256];

int  line_len = 0;

int sd;  /* socket file descriptor */

read_socket(fd,id)
    int   fd;
    void *id;
{
    int n = read(fd,in_buff,255);
    if ( n==0 ) {
        inform_done();
        fprintf(stderr,"socket closed\n");
        return 0;
        }
    in_buff[n] = '\0';
    printf("received %s\n",in_buff);
    fflush(stdout);
    return 0;
}

term_line(fd,id,buff)
    int   fd;
    void *id;
    char *buff;
{
    mess("sending {%s}\n",buff);
    write(sd,buff,strlen(buff));
    return 0;
}

term_eof(fd,id)
    int   fd;
    void *id;
{
    inform_done();
    mess("client done\n");
    return 0;
}

#define	PORTNO	6789
#define HOST    "gamma"

int port = PORTNO;
char *host = HOST;

main(argc,argv)
    int argc;
    char **argv;
{
    char *errmess;

    if ( parse_network_args(&argc,argv,&host,&port,&errmess) != 0 ) {
	fprintf(stderr,"%s: %s\n",argv[0],errmess);
	exit(1);
	}
    sd = tcp_active_open(host,port);
    if ( sd < 0 ) { perror("socket"); exit(1); }
    printf("You can send now\n");
    inform_line_by_line(0,term_line,term_eof,NULL);
    inform_input(sd,read_socket,NULL);
    inform_loop();
    exit(0);
}

