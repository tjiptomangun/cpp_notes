/*http://www.tidytutorials.com/2010/06/linux-c-socket-example-with-client.html*/
/* CC -Wall -ggdb3 XLAdm.c -o XLAdm */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void usage ()
{
	/* fprintf ( stderr , "usage : XLAdmin [Start|Stop|Terminate|Kill]\n");*/
	 fprintf ( stderr , "usage : XLAdmin [Start|Stop|Terminate]\n");
	 fprintf ( stderr , "where   Start to start XLink, Stop to stop XLink, \n");
	 fprintf ( stderr , "        Terminate to terminate XLink script.\n");
}
#define MODE_START 1
#define MODE_STOP  2 
#define MODE_TERMINATE 3 
#define MODE_KILL  4
int main ( int argc, char **argv )
{
	int host_port = 1101;
	char host_name[] = {"127.0.0.1"};

	struct sockaddr_in my_addr;
	int mode = 0;

	char buffer[1024];
	int bytecount = 0;
	int buffer_len = 0;

	int hsock;
	int *p_int;
	int err;

	if ( argc < 2 )
	{
		usage();
		exit ( 1 );
	}
	
	if ( strcmp (argv [ 1 ] , "Start" ) && strcmp ( argv [ 1 ], "Stop" ) 
	   && strcmp (argv [ 1 ] , "Terminate" ) && strcmp ( argv [ 1 ], "Kill" ) )
	{
		usage();
		exit ( 1 );	
	}
	mode = MODE_STOP ;	
	if ( ! strcmp ( argv [ 1 ], "Start" ) )
	{
		mode = MODE_START ;
	}
	if ( ! strcmp ( argv [ 1 ], "Terminate" ) )
	{
		mode = MODE_TERMINATE;
	}
	if ( ! strcmp ( argv [ 1 ], "Kill" ) )
	{
		mode = MODE_KILL;
	}

	hsock = socket ( AF_INET, SOCK_STREAM , 0 );
	if ( hsock == -1 )
	{
		printf ("error initializing socket %d\n", errno );
		exit (1 );
	}

	p_int = (int *) calloc (1, sizeof (int ));
	*p_int = 1;

	
	if ( ( setsockopt ( hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, sizeof( int )) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, sizeof( int )) == -1))
	{
		fprintf ( stderr, "error setting options %d\n", errno );
		free (p_int );
		close ( hsock );
		exit (1);
	}
	
	free ( p_int);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(host_port);

	memset ( &(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr ( host_name );

	if ( connect ( hsock, ( struct sockaddr * ) &my_addr, sizeof ( my_addr )) == -1 )
	{
		if ( ( err = errno ) != EINPROGRESS )
		{
			fprintf ( stderr, "error connecting socket %d\n", errno);
			close ( hsock );
			exit ( 1 );
		}
	}

	// Now let do the client related stuff

	buffer_len = 1024 ;
	memset ( buffer, 0 , buffer_len );

	memset ( buffer, 0 , buffer_len );
	sprintf ( buffer, "sign on" );

	if ( ( bytecount = send ( hsock, buffer, strlen( buffer ), 0 )) == -1 )
	{
		fprintf (stderr, "error sending data %d\n", errno );
		close ( hsock );
		exit ( 1 );
	}
	/* fprintf (stdout,  "sent bytes %d\n", bytecount ); */
	/* while ( 1 ) */
	/* { */
	memset ( buffer, 0 , buffer_len );
	if ( ( bytecount = recv ( hsock, buffer, buffer_len, 0)) == -1 )
	{
		fprintf (stderr, "error receiving data %d\n", errno );
		close ( hsock );
		exit ( 1 );
	}
	/* printf ("received bytes %d\nReceiving string \"%s\"\n", bytecount, buffer); */
	memset ( buffer, 0 , buffer_len );
	/* fgets ( buffer, 1024 , stdin ); */
	sprintf ( buffer ,"%d", mode );
	/* buffer [strlen ( buffer ) -1 ] ='\0'; */
	/*	
	if ( !strncmp ( buffer , "quit", 4) ) 
	{
		close ( hsock );
		exit ( 0) ;
	}
	*/

		/* fprintf (stdout,  "sent bytes %d\n", bytecount ); */
	/* } */
	if ( ( bytecount = send ( hsock, buffer, strlen( buffer ), 0 )) == -1 )
	{
		fprintf (stderr, "error sending data %d\n", errno );
		close ( hsock );
		exit ( 1 );
	}

	memset ( buffer, 0 , buffer_len );
	if ( ( bytecount = recv ( hsock, buffer, buffer_len, 0)) == -1 )
	{
		fprintf (stderr, "error receiving data %d\n", errno );
		close ( hsock );
		exit ( 1 );
	}
	printf ("\n\"%s\"\n",  buffer);

	close ( hsock );
	exit ( 0) ;
}
