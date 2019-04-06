/*http://www.tidytutorials.com/2010/06/linux-c-socket-example-with-client.html*/
/* CC -Wall -ggdb3 xladm.c -o xladm */

#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "parserclass.h"
#include "filecfgparser.h"

int host_port = 1101;
void usage ()
{

	 fprintf (stderr , "usage : xladm [start|stop|kill]\n");
	 fprintf (stderr , "where   start to start XLink, stop to stop XLink, \n");
	 fprintf (stderr , "        kill to kill XLink script.\n");
	 fprintf (stderr , "all options are case insensitive.\n");
}
#define MODE_START 1
#define MODE_STOP  2 
#define MODE_TERMINATE 3 
#define MODE_KILL  4

int PrepareService ()
{
	FILE *fp;
	PMINIPARSER pfile;
	char tokenlist [] = "[]#\n=";
	PLIST root = 0;
	PLIST curr = 0;
	PPROPERTY pprop = 0; 

	fp = fopen ("../dat/xladmd.ini", "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file ../dat/xladmd.ini\n");
		exit (1);
	} 

	pfile = newminiparser (fp, tokenlist, infile_parse);
	pfile->parse (pfile, &root);
	fclose (fp); 
	
	curr = (PLIST) root->takename (root, "process"); 
	pprop = (PPROPERTY) curr->takename (curr, "PORT"); 
	if (pprop)
	{
		if (strlen (pprop->value))
			host_port = atoi (pprop->value);

		pprop->l_item.class.preparedelete (&pprop->l_item.class);
		free (pprop);
	}
	

	root->l_item.class.preparedelete (&root->l_item.class);
	free (root); 

	return 0;
}/*PrepareService*/

int main (int argc, char **argv )
{
	char host_name[] = {"127.0.0.1"};

	struct sockaddr_in my_addr;
	int mode = 0;

	char buffer[1024];
	int bytecount = 0;
	int buffer_len = 0;

	int hsock;
	int *p_int;
	int err; 

	if (argc < 2 )
	{
		usage();
		exit (1 );
	}

	PrepareService ();
	
	if (strcasecmp (argv [ 1 ] , "start" ) && strcasecmp (argv [ 1 ], "stop" ) 
	   && strcasecmp (argv [ 1 ] , "terminate" ) && strcasecmp (argv [ 1 ], "kill" ) )
	{
		usage();
		exit (1 );	
	}
	mode = MODE_STOP ;	
	if (! strcasecmp (argv [ 1 ], "start" ) )
	{
		mode = MODE_START ;
	}
	if (! strcasecmp (argv [ 1 ], "terminate" ) )
	{
		mode = MODE_TERMINATE;
	}
	if (! strcasecmp (argv [ 1 ], "kill" ) )
	{
		mode = MODE_KILL;
	}

	hsock = socket (AF_INET, SOCK_STREAM , 0 );
	if (hsock == -1 )
	{
		printf ("error initializing socket %d\n", errno );
		exit (1 );
	}

	p_int = (int *) calloc (1, sizeof (int ));
	*p_int = 1;

	
	if ((setsockopt (hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, sizeof(int )) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, sizeof(int )) == -1))
	{
		fprintf (stderr, "error setting options %d\n", errno );
		free (p_int );
		close (hsock );
		exit (1);
	}
	
	free (p_int);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(host_port);

	memset (&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr (host_name );

	if (connect (hsock, (struct sockaddr * ) &my_addr, sizeof (my_addr )) == -1 )
	{
		if ((err = errno ) != EINPROGRESS )
		{
			fprintf (stderr, "error connecting socket %d\n", errno);
			close (hsock );
			exit (1 );
		}
	}

	// Now let do the client related stuff

	buffer_len = 1024 ;
	memset (buffer, 0 , buffer_len );

	/* Protocol */
	sprintf (buffer, "sign on" );

	if ((bytecount = send (hsock, buffer, strlen(buffer ), 0 )) == -1 )
	{
		fprintf (stderr, "error sending data %d\n", errno );
		close (hsock );
		exit (1 );
	}
	
	memset (buffer, 0 , buffer_len );
	if ((bytecount = recv (hsock, buffer, buffer_len, 0)) == -1 )
	{
		fprintf (stderr, "error receiving data %d\n", errno );
		close (hsock );
		exit (1 );
	}

	memset (buffer, 0 , buffer_len ); 
	sprintf (buffer ,"%d", mode ); 

	if ((bytecount = send (hsock, buffer, strlen(buffer ), 0 )) == -1 )
	{
		fprintf (stderr, "error sending data %d\n", errno );
		close (hsock );
		exit (1 );
	}

	memset (buffer, 0 , buffer_len );
	if ((bytecount = recv (hsock, buffer, buffer_len, 0)) == -1 )
	{
		fprintf (stderr, "error receiving data %d\n", errno );
		close (hsock );
		exit (1 );
	}
	printf ("\n\"%s\"\n",  buffer);

	close (hsock );
	/* Protocol Done */

	exit (0) ;
}
