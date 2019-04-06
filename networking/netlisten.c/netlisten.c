#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc,char **argv[])
{
    int nPort=0;
    int nProt=0;
    int nSkt;
    struct sockaddr_in 	Local;
    printf("argc=%d\n",argc);
    if(argc<3)
    {
		printf("usage netlisten protocol port\n");
        printf("where protocol = 0 upd, other value tcp\n");
		exit(0);
    }
    nProt = atoi((char *)argv[1]);
    nPort = atoi((char *)argv[2]);
    printf("argv[1]=%s Prot=%d\n",argv[1],nProt);
    printf("argv[2]=%s Port=%d\n",argv[2],nPort);
	if ((nSkt = socket (AF_INET, nProt==0?SOCK_DGRAM:SOCK_STREAM, 0)) < 0) 
	{
		printf("error create socket\n");
	}
	Local.sin_family 		= AF_INET;
	Local.sin_addr.s_addr 	= htonl (INADDR_ANY);
	Local.sin_port 			= htons (nPort);
	if (bind (nSkt, (struct sockaddr *) &Local, sizeof Local) < 0)
	{
		printf("error binding socket\n");
	}
	while(1)
    {
        
        sleep(1);
    }
}