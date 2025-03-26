#ifndef _TCP_H_
#define _TCP_H_
#define ERR_TCP_TIMEOUT (-1000)
#define SOCK_TIMEOUT (15)

typedef struct _TCPMSG {
        unsigned int len;
        unsigned int sequence;
        unsigned int command;
        unsigned int status;
        char data[1024];
        unsigned int datalen;
} TCPMSG;

typedef struct {
	char client_ip[30];
	unsigned int server_port;	
	int sock;
} TCPPARAM;

int Recv(int sock,char *buffer,int len,int flag,int sec);
int TCPSendLV(int sock,unsigned char *data,int msglen,int flag);
int TCPRecvLV(int sock,unsigned char *data,int maxlen,int flag);
int ConnectWithTimeout(char *server_addr,int port,int timeout);
#endif