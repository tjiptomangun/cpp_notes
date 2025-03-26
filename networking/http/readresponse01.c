//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
	perror(msg);
	exit(0);

}
int main(int argc, char *argv[]) {
	/* first we are going to send and where are we going to send it? */
	int portno = 3001;
	char *host = "ubunturumah.com";
	char *message_fmt= "POST /apikey=%s&command=%s HTTP/1.0\r\n\r\n";

	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total;

	char message[1024], response [4096];

	if (argc < 3) {
		puts("Parameter: <apikey> <command>");
		exit(0);
	}

	sprintf(message, message_fmt, argv[1], argv[2]);
	printf("Request:\n%s\n", message);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <0) {
		error("ERROR Openning socket");
	}

	server = gethostbyname(host);
	if (server == NULL) {
		error("ERROR no such host");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port= htons(portno);
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	/*connect to the socket*/
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}

	total = strlen(message);
	sent = 0;
	do {
		bytes = write(sockfd, message + sent, total - sent);
		if (bytes < 0) {
			error("ERROR writing message to socket");
		}
		if (bytes == 0) {
			break;
		}
		sent += bytes;
	} while(sent < total);

	memset(response, 0, sizeof(response));
	total = sizeof(response) - 1;
	received = 0;
	do {
		bytes = read(sockfd, response + received, total - received);
		if (bytes < 0) {
			error("ERROR reading response from socket");
		}
		if (bytes == 0) {
			break;
		}
		received += bytes;
	} while(received < total);

	/*
	 * if the number of received bytes is the total size of the
	 * array then we have run out of space to store the response
	 * and it hasn't all arrived yet - so that's a bad thing
	 */

	if(received == total){
		error("ERROR storing complete response from socket");
	}

	close(sockfd);

	printf("Response: \n%s\n", response);
	char *body = strstr(response, "\r\n\r\n");
	printf("body: \n%s\n", body); 

	return 0;
}
