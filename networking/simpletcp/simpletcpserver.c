#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main ( void )
{
	struct sockaddr_in stSockAddr;
	int	SocketFD = socket (PF_INET, SOCK_STREAM , IPPROTO_TCP );

	if ( SocketFD == -1 )
	{
		perror ("can not create socket\n");
		exit (EXIT_FAILURE );

	}

	memset (&stSockAddr, 0, sizeof (stSockAddr ));
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port   = htons(1100);
	stSockAddr.sin_addr.s_addr = INADDR_ANY ;

	if ( bind (SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr ) == -1)
	{
		perror("error bind failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	if ( listen (SocketFD, 10 ) )
	{
		perror ("error listen failed ");
		close(SocketFD);
		exit(EXIT_FAILURE);	
	}
	for (;;)
	{
		int ConnectFD = accept(SocketFD, NULL, NULL );
		if (ConnectFD < 0)
		{
			perror("error accept failed ");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
		printf ("somebody is trying\n");
		shutdown(ConnectFD, SHUT_RDWR );
		close(ConnectFD);
		printf ("shutdown and close\n");
		
	}
	close(SocketFD);
	return 0;

}
