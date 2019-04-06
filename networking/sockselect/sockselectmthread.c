/**
 * $CC -Wall -ggdb3 stdxuiparser.c parserclass.c  sockselectmthread.c -lpthread -o sockselectmthread 
 *
 * send various xuimessage format from command interpretter
 */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "parserclass.h"
#include "stdxuiparser.h"

int hsock;
char host_name [] = {"127.0.0.1"};
int port = 10000; 
PLIST root;
int make_connecting_socket (char *host_name, int port)
{ 
	struct sockaddr_in addr;
	int hsock;
	int sock_optval;

	hsock = socket (AF_INET, SOCK_STREAM, 0);
	if (hsock == -1) 
		return -1;

	sock_optval = 1;
	if ((setsockopt (hsock, SOL_SOCKET, SO_REUSEADDR, 
		(void *)&sock_optval, sizeof (int))==-1)||
	   (setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE,
		(void *)&sock_optval, sizeof (int))==-1)) 
	{
		close (hsock);
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons (port);
	
	memset (&addr.sin_zero, 0, 8);
	addr.sin_addr.s_addr = inet_addr (host_name);

	if (connect (hsock, (struct sockaddr *)&addr, sizeof (addr)) == -1)
	{
		close (hsock);
		return -1;
	} 
	return hsock;
}

void * sockread_handle (void *lp)
{
	fd_set *tmp = (fd_set *) lp;	
	fd_set readfd = *tmp;
	struct timeval tv;
	tv.tv_sec  = 1;
	tv.tv_usec = 0;

	int ret;
	do
	{
		ret = select (hsock + 1, &readfd, 0, 0, &tv);
		if (ret < 0)
		{
			perror ("read"); 
			continue;
		}
		if (FD_ISSET (hsock, & readfd))
		{
			//read the available data here
			

		}
		
	}while (1==1);
}

void * sockwrite_handle (void *lp)
{ 
	fd_set *tmp = (fd_set *) lp;	
	fd_set writefd = *tmp;
	struct timeval tv;
	tv.tv_sec  = 1;
	tv.tv_usec = 0; 
	int ret;
	do
	{
	//	fprintf (stdout, "zwick\n");
		if (!root->take(root))
		{
			sleep (1);
			continue;
		}
		
		ret = select (hsock + 1, 0, &writefd, 0, &tv);
		if (ret < 0)
		{
			perror ("write"); 
			continue;
		}
		if (FD_ISSET (hsock, & writefd))
		{
			write (hsock, "Hello World!", 12);

		}
		
	}while (1==1);
}

void * stdin_handle (void *lp)
{
	PMINIPARSER pstdio;
	fprintf (stdout, "newminiparser\n");
	pstdio = newminiparser (stdin, " \n(,)", stdin_parse);
	pstdio->parse (pstdio, &root); 
	return NULL;
}

int main (int argc, char **argv)
{
	int err; 
	struct timeval tv;
	fd_set active_fd_set;
	pthread_t read_thread = 0, write_thread = 0, command_thread = 0;
	root = newlist ("root");
		
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	hsock = make_connecting_socket (host_name, port); 
	if (hsock == -1)
	{
		perror ("setsockopt");
		exit (EXIT_FAILURE); 
	}

	FD_ZERO (&active_fd_set);
	FD_SET (hsock, &active_fd_set);
	
	/*
	 * dispatch writter thread
	 * dispatch reader thread
	 * dispatch command interpretter
 	 */
	pthread_create (&read_thread, 0, &sockread_handle, (void *)&active_fd_set); 
	pthread_detach (read_thread);
	pthread_create (&write_thread, 0, &sockwrite_handle, (void *)&active_fd_set); 
	pthread_detach (write_thread);
	pthread_create (&command_thread, 0, &stdin_handle, NULL); 
	pthread_join (command_thread, NULL);

	

	exit (EXIT_SUCCESS);	
}
