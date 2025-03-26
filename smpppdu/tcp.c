#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/timeb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "logger.h"
#include "memcheck.h"
#include "tcp.h"


pthread_mutex_t lock_sendLV[1000]=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_timeLV[1000]=PTHREAD_MUTEX_INITIALIZER;
time_t timeLV[1000]={0};

int connectWithTimeout(int soc,struct  sockaddr *name,int namelen,int timeout) {
	int res, valopt;
	long arg;
	fd_set myset;
	struct timeval tv;
	socklen_t lon;

	/* Set non-blocking */
	arg = fcntl(soc, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(soc, F_SETFL, arg);

	/* Trying to connect with timeout */
	res = connect(soc,name,namelen);
	if (res < 0) {
		if (errno == EINPROGRESS) {
			tv.tv_sec = timeout;
		        tv.tv_usec = 0;
		        FD_ZERO(&myset);
		        FD_SET(soc, &myset);
		        if (select(soc+1, NULL, &myset, NULL, &tv) > 0) {
				lon = sizeof(int);
				getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
				if (valopt) {
					/*fprintf(stderr, "Error in connection() %d - %s\n", valopt, strerror(valopt));*/
					close(soc);
					return(-1);
				}
			} else {
				/*fprintf(stderr, "Timeout or error() %d - %s\n", valopt, strerror(valopt));*/
				close(soc);
				return(-1);
			}
		} else {
			/*fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));*/
			close(soc);
			return(-1);
		}
	}
	/* Set to blocking mode again... */
	arg = fcntl(soc, F_GETFL, NULL);
	arg &= (~O_NONBLOCK);
	fcntl(soc, F_SETFL, arg);
	return(soc);
}

int ConnectWithTimeout(char *server_addr,int port,int timeout) {
        int sock;
        int ret=-1;
        struct sockaddr_in server;
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock <0) {
                exit(1);
        }
        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr(server_addr);
        server.sin_port=htons(port);
        ret=connectWithTimeout(sock,(struct sockaddr *)&server,sizeof(server),timeout);
        if(ret<0) {
                close(sock);
                sock=-1;
        }
        return(sock);
}


int TCPServer(int port,void *func) {
        struct sockaddr_in local_sin;  /* Local socket - intenet style */
        struct sockaddr_in acc_sin;    /* Accept socket address - intenet style */
        socklen_t acc_sin_len;        /* Accept socket address length */
        int reuse_addr=1;
        int sock;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        int csock;
	TCPPARAM *param;


        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        sock=socket(AF_INET,SOCK_STREAM,0);
        if (sock == -1) {
        printf("Can not create socket\n");
                exit(1);
        }
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse_addr,sizeof(int));
        local_sin.sin_family=AF_INET;
        local_sin.sin_addr.s_addr =INADDR_ANY;
        local_sin.sin_port=htons(port);
        if (bind( sock, (struct sockaddr *) &local_sin,sizeof(local_sin)) <0) {
                printf("Can not bind socket\n");
                exit(1);
        }
        if (listen( sock, 50 ) < 0) {
                printf("Can not listen socket\n");
                exit(1);
        }
        acc_sin_len=sizeof(acc_sin);
        while(1)//we are looping endlessly
        {
                csock=accept(sock,(struct sockaddr *)&acc_sin,&acc_sin_len);
                if(csock<0) continue;
		param=malloc(sizeof(TCPPARAM));
                param->sock=csock;
		param->server_port=port;
		sprintf(param->client_ip,"%s",inet_ntoa((struct in_addr)acc_sin.sin_addr));
                pthread_create(&tid,&attr,func,(void *)param);
        }
}

int Recv(int sock,char *buffer,int len,int flag,int sec) {
        int ret;
        int nb;
        fd_set socks;
        struct timeval timeout;
        while(1) {
                FD_ZERO(&socks);
                FD_SET(sock,&socks);
                timeout.tv_sec=sec;
                timeout.tv_usec=0;
                nb=select(FD_SETSIZE,&socks,(fd_set *)0,(fd_set *)0,&timeout);
                if(nb<0) {
                        if(errno==EINTR) continue;
                        return(-1);
                } else if(nb==0) {
                        return(ERR_TCP_TIMEOUT);
                }
                if(FD_ISSET(sock,&socks)) {
                        ret=recv(sock,buffer,len,flag);
                        if(ret<0 && errno==EINTR) continue;
                        break;
                }
        }
        return(ret);
}

int Connect(char *server_addr,int port) {
	int sock;
	int ret=-1;
	struct sockaddr_in server;
	sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock <0) {
        	exit(1);
        }
        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr(server_addr);
        server.sin_port=htons(port);
        ret=connect(sock,(struct sockaddr *)&server,sizeof(server));
        if(ret<0) {
        	close(sock);
		sock=-1;
	}
	return(sock);
}

int TCPRecv(TCPMSG *tmsg,int sock,int en_to) {
        int ret;
        unsigned char buffer[1024];
        int plen;
        int alen=0;
        int len;

        ret=Recv(sock,(char *)buffer,1,0,SOCK_TIMEOUT);
        if(en_to==0 && ret==ERR_TCP_TIMEOUT) return(0);
        if(ret!=1) {close(sock);return(-1);}
        plen=256*buffer[0];

        ret=Recv(sock, (char *) buffer,1,0,SOCK_TIMEOUT);
        if(ret!=1) {close(sock);return(-1);}
        plen+=buffer[0];

        if(plen<8 || plen > 1024) {close(sock);return(-1);}

        alen=2;

        while(alen!=plen) {
                len=Recv(sock,(char *) (&buffer[alen-2]),plen-alen,0,SOCK_TIMEOUT);
                if(len<=0) {close(sock);return(-1);}
                alen+=len;
        }

        tmsg->len=plen;
        tmsg->sequence=buffer[0]*256+buffer[1];
        tmsg->command=buffer[2]*256+buffer[3];
        tmsg->status=buffer[4]*256+buffer[5];
        tmsg->datalen=plen-8;
        memcpy(tmsg->data,&buffer[6],tmsg->datalen);
        tmsg->data[tmsg->datalen]=0;
        return(plen);

}

int TCPSend(int sock,TCPMSG *tmsg) {
        int ret;
        unsigned char buffer[1024];
        int plen;
        if(tmsg->datalen<0 || tmsg->datalen>1000) {close(sock);return(-1);}
        plen=tmsg->datalen+8;
        buffer[0]=plen/256;
        buffer[1]=plen%256;
        buffer[2]=(tmsg->sequence)/256;
        buffer[3]=(tmsg->sequence)%256;
        buffer[4]=(tmsg->command)/256;
        buffer[5]=(tmsg->command)%256;
        buffer[6]=(tmsg->status)/256;
        buffer[7]=(tmsg->status)%256;
        memcpy(&buffer[8],tmsg->data,tmsg->datalen);
        ret=send(sock,buffer,plen,0);
        if(ret!=plen) {
                close(sock);
                return(-1);
        }
        return(ret);
}

int TCPRecvLV(int sock,unsigned char *data,int maxlen,int flag) {
	int ret,alen;	
	unsigned int len;
	unsigned char temp;
	ret=Recv(sock,(char *) (&temp),1,flag,65);
	if(ret<=0) return(ret);
	len=256*temp;
	ret=Recv(sock, (char *) (&temp),1,flag,5);
	if(ret<=0) return(ret);
	len+=temp;
	if(len<=0 || len>1000 || len>maxlen) {
		printf("len error %d\n",len);
		return(-1);
	}
	alen=0;
	do {
		ret=Recv(sock,(char *)  (&data[alen]),len-alen,flag,10);
		if(ret<=0) return(-1);
		alen+=ret;
	} while(alen<len);
	if(alen!=len) return(-1);
	data[alen]=0x00;
	return(alen);
}

int TCPSendLV(int sock,unsigned char *data,int msglen,int flag) {
	time_t now;
	unsigned char buffer[1200];
	int ret;
	if(sock>=1000)  {
		shutdown(sock,SHUT_RDWR);
		return(-1);
	}
	if(msglen<=0 || msglen>1000) {
		shutdown(sock,SHUT_RDWR);
		return(-1);
	}
	buffer[0]=(unsigned char)(msglen/256);
	buffer[1]=(unsigned char)(msglen%256);
	memcpy(&buffer[2],data,msglen);
	pthread_mutex_lock(&lock_timeLV[sock]);
	if(timeLV[sock]!=0) {
		time(&now);
		if(now-timeLV[sock]>3) {
			shutdown(sock,SHUT_RDWR);
			pthread_mutex_unlock(&lock_timeLV[sock]);
			return(-1);
		}
	} else {
		time(&timeLV[sock]);
	}
	pthread_mutex_unlock(&lock_timeLV[sock]);
	pthread_mutex_lock(&lock_sendLV[sock]);
	ret=send(sock,buffer,msglen+2,flag);
	pthread_mutex_unlock(&lock_sendLV[sock]);
	pthread_mutex_lock(&lock_timeLV[sock]);
	timeLV[sock]=0;
	pthread_mutex_unlock(&lock_timeLV[sock]);
	return(ret);
}

