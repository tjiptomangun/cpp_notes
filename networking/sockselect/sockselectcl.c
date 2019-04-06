/*
 * sockselectcl.c : sockselect client
 * gcc -Wall -ggdb3 sockselectcl.c -o sockselectcl
 */
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT 5555
#define MAXMSG 512


int read_from_remote (int filedes)
{
	char buffer [MAXMSG];
	int nbytes;
	memset (buffer, 0, MAXMSG);

	nbytes = read (filedes, buffer, MAXMSG);
	if (nbytes < 0)
	{
		/* Read error */
		perror ("read");
		exit (EXIT_FAILURE);
	}
	else if (nbytes == 0)
	{
		/* End of file */
		return -1; 
	}
	else
	{
		/* Data read */
		fprintf (stderr, "Server : got message: '%s'\n", buffer);
		return 0;
	}
}/* end of read_from_remote */

int main (int argc, char *argv[])
{
	char host_name [] = {"127.0.0.1"};
	struct sockaddr_in my_addr;
	char buff[MAXMSG];

	int bytecount = 0;
	int buffer_len = 0;
	int hsock;
	int err;
	int *p_int;
	int port = PORT;
	struct timeval tv;
	int packets = 0;
	int ret;

	if (argc > 1)
	{
		port = atoi (argv[1]);
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	fd_set active_fd_set, read_fd_set, write_fd_set;

	hsock = socket (AF_INET, SOCK_STREAM, 0); /* create the socket descriptor */
	if (hsock == -1)
	{
		fprintf (stderr, "error initializing socket %d\n", errno);
		exit (EXIT_FAILURE);
	} 
	p_int = (int *) calloc (1, sizeof (int));
	*p_int = 1;

	if ((setsockopt (hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, sizeof (int)) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, sizeof (int)) == -1))
	{
		fprintf (stderr, "error setting options %d\n", errno);
		free (p_int);
		close (hsock);
		exit (1);
	}
	free (p_int);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons (port);
	
	memset (&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr= inet_addr (host_name);

	if (connect (hsock, (struct sockaddr *) &my_addr, sizeof (my_addr)) == -1)
	{
		if ((err = errno) != EINPROGRESS)
		{
			fprintf (stderr, "error connection socket %d\n", errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
	}
	while (1)
	{

		FD_ZERO (&active_fd_set);
		FD_SET (hsock, &active_fd_set);
		
		write_fd_set = active_fd_set;
		/* select returns 0 if timeout, -1 if error*/
		/* http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html */
		if ((ret = select (hsock + 1, NULL, &write_fd_set, NULL, &tv))<0)
		{
			fprintf (stderr, "error write select %d errno %d", ret, errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
		else if (ret == 0)
		{
			//no bits set
			fprintf (stderr, "no bits set: timeout write\n"); 
			sleep (2);
		}
		else
		{
			if (!packets)
			{
				write (hsock, "sign on", 7);
				packets ++;
			}
		}
		read_fd_set = active_fd_set;
		if ((ret = select (hsock + 1, &read_fd_set, NULL, NULL, &tv))<0)
		{
			fprintf (stderr, "error write select %d errno %d", ret, errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
		else if (ret == 0)
		{
			//no bits set
			fprintf (stderr, "no bits set: timeout read\n");
			sleep (2);
		}
		else
		{
			if (packets)
			{
				read_from_remote (hsock);
				packets --;
			}
		}
	} 
}

