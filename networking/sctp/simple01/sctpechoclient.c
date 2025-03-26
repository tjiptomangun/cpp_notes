//SCTPClient.C
// To compile - gcc sctpclt.c -o client -lsctp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#define MAX_BUFFER 1024
#define MY_PORT_NUM 62324 /* This can be changed to suit the need and should be same in server and client */

int
main (int argc, char* argv[])
{
  int connSock, ret, portno; 
  struct hostent *server;
  struct sockaddr_in servaddr;
  // struct sctp_status status;
  char buffer[MAX_BUFFER + 1] = {0};
  int datalen = 0;
  if (argc < 3) {
      fprintf(stderr,"usage: %s hostname port\n", argv[0]);
      exit(0);
  } else if (argc > 3) {
      strncpy(buffer, argv[3], MAX_BUFFER);
  }
  server = gethostbyname(argv[1]);
  portno = atoi(argv[2]); //Convert ASCII to integer


  /*Get the input from user*/
  /* Clear the newline or carriage return from the end*/
  /* Sample input */
  //strncpy (buffer, "Hello Server", 12);
  //buffer[12] = '\0';

  connSock = socket (AF_INET, SOCK_STREAM, IPPROTO_SCTP);

  if (connSock == -1)
  {
      printf("Socket creation failed\n");
      perror("socket()");
      exit(1);
  }

  bzero ((void *) &servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (portno);
  bcopy((char *)server->h_addr, 
    (char *)&servaddr.sin_addr.s_addr,
    server->h_length);

  ret = connect (connSock, (struct sockaddr *) &servaddr, sizeof (servaddr));

  if (ret == -1)
  {
      printf("Connection failed\n");
      perror("connect()");
      close(connSock);
      exit(1);
  }
  datalen = strlen(buffer);
  do {

      if (datalen) {
        ret = sctp_sendmsg (connSock, (void *) buffer, (size_t) datalen,
              NULL, 0, 0, 0, 0, 0, 0);
        if(ret == -1 )
        {
          printf("Error in sctp_sendmsg\n");
          perror("sctp_sendmsg()");
        }
        else {
            printf("Successfully sent %d bytes data to server\n", ret);
        }
      }
     sleep(1);
    
  } while (1 == 1);
  printf("toexit but why ? \n");
  close (connSock);

  return 0;
}


