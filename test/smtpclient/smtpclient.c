#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    while(1)
    {
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	bzero(buffer,256);
	
	/*------------------------------*/
	
	printf("EHLO");
	
	strcpy(buffer,"ehlo smtp.infolink.co.id\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,255);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	/*------------------------------*/    
	
	/*------------------------------*/
	
	printf("AUTH");
	
	strcpy(buffer,"AUTH LOGIN\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	/*------------------------------*/ 
	
	/*------------------------------*/
	
	printf("AUTH UID");
	
	strcpy(buffer,"amR1bnBoeUBwcmltdXMuY2E=\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	
	/*------------------------------*/ 
	
	/*------------------------------*/
	
	printf("AUTH PWD");
	
	strcpy(buffer,"YXAwbGwwMTE=\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	/*------------------------------*/ 
	
	/*------------------------------*/
	
	printf("MAIL FROM");
	
	strcpy(buffer,"MAIL FROM: henky@infolink.co.id\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	/*------------------------------*/ 
	
	/*------------------------------*/
	
	printf("MAIL TO");
	
		
	strcpy(buffer,"RCPT TO: henky.ac@gmail.com\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	/*------------------------------*/       
	
	/*------------------------------*/
	
	printf("DATA");
	
	strcpy(buffer,"DATA\r\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	
	strcpy(buffer,"Subject: test\r\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	
	strcpy(buffer,"SMTP MAIL TOOL TEST WORKS!!!\r\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	
	strcpy(buffer,"\n\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	
	strcpy(buffer,".\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	
	
	/*------------------------------*/ 
	
	/*------------------------------*/ 
	
	strcpy(buffer,"quit\n");
	
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",buffer);
        close(sockfd);
	/*------------------------------*/
        sleep(1);
   }
   return 0;
}
