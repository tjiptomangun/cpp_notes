// https://stackoverflow.com/q/6798967
// https://stackoverflow.com/a/6799065
// https://stackoverflow.com/a/16154717

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
/* the next two includes probably aren't relevant for you, but I typically use them all anyway */
#include <math.h>
#include <sys/termios.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct packet
    { 
        long int srcID;
        long int destID;
        long int pver;
        long int profiles;
        char length;
        long int data;
    };
    if (argc < 3) {
        fprintf(stderr,"usage: %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]); //Convert ASCII to integer
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket file descriptor

    if (sockfd < 0) 
        error("ERROR DETECTED !!! Problem in opening socket\n");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR DETECTED !!!, no such server found \n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //clear the memory for server address

    serv_addr.sin_family = AF_INET;    
    bcopy((char *)server->h_addr, 
    (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);

    serv_addr.sin_port = htons(portno);

    printf("Client 1 trying to connect with server host %s on port %d\n", argv[1], portno); 

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR in connection");

    printf("SUCCESS !!! Connection established \n");

    char buffer[128];
    struct packet *pkt = (struct packet *) buffer;
    char *payload = buffer + sizeof(struct packet);
    long int packet_size;

    printf("Started Creating packet\n");
    pkt->srcID = 0x01;
    pkt->destID = 0x02;
    pkt->pver = 0x01;
    pkt->profiles = 0x01;
    pkt->length = 128;
    pkt->data = 1; 2; 3; 4; 5; 6; 7; 8;
    
    while (1) {

        if (send(sockfd,pkt,sizeof(packet_size),0) <0){
			printf ("error\n");
	  	    exit(1);
		} else {
			printf ("packet send done");
		}
		sleep(1);
    }
    return 0;
}
