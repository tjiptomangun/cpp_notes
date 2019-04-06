#ifndef __RRCLUSTER_H__
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "parserclass.h"

#pragma pack(1)
typedef struct cnode_prop
{
	// bit 7 - connection status
	// bit 6 - sign on status
	// bit 5 - thread initialized
	// bit 4 - reserved
	// bit 3 - reserved
	// bit 2 - reserved
	// bit 1 - reserved
	// bit 0 - reserved
	unsigned char status;
	int nodesd;
	int id;
	pthread_t node_tid;
	pthread_attr_t t_node_attr;
	sem_t node_sem;
	pthread_mutex_t mx_fd;
}CNODE_PROP, *PCNODE_PROP;
pthread_attr_t t_node_attr; // cluster node common thread attribute
#pragma pack()

#define NODE_STAT_DISCONNECT    0x10
#define NODE_STAT_CONNECTION	0x80
#define NODE_STAT_SIGNON	0x40
#define NODE_STAT_THREAD	0x20

#define NODE_STAT_ACTIVE	0xE0
#endif
#define BUFF_UNIT 80

int x_fprintf (FILE *out, char *fmt, ...)
{
	struct timeb	timeB;	
	time_t now;
	struct tm tmnow;
	time (&now);
	gmtime_r (&now, &tmnow);
	

	char timeexec [BUFF_UNIT];
	va_list args;
	va_start (args, fmt); 
	
	memset (timeexec, 0, BUFF_UNIT);
	ftime (&timeB); 
        sprintf (timeexec, "%02d %02d %02d %8.8s  ", tmnow.tm_year-100, tmnow.tm_mon+1,
		tmnow.tm_mday, ctime(&timeB.time)+11);

	fprintf (out, timeexec);

	vfprintf (out, fmt, args);
	fflush (out);
	
	return (0);
		
}

void node_setstatus(CNODE_PROP *node, unsigned char param) {
  node->status |= param;
}

void node_unsetstatus(CNODE_PROP *node, unsigned char param) {
  node->status &= ~param;
}

int node_isset(CNODE_PROP *node, unsigned char param) {
  return (node->status & param);
}
#define SOCK_BUFFER_SIZE 100
void * node_init ()
{//remove from list if status already disconnet

}

int cluster_waitrq (CNODE_PROP *n, unsigned char *buf)
{
	int readlen = 0;
	unsigned short len = -1;
	int r;
	int fd = n->nodesd;

	readlen = read (fd, buf, 1);
	r = readlen;
	while (readlen >= 0)
	{

		readlen = read (fd, buf, 1); 
	}
	return 0;
}
void * node_handler (void *p)
{
	PCNODE_PROP nprop = p;
	int res = 0;
	unsigned char lbuf[SOCK_BUFFER_SIZE];

	cluster_waitrq (nprop, lbuf);

	node_unsetstatus (p, NODE_STAT_CONNECTION);
	node_unsetstatus (p, NODE_STAT_SIGNON);
	node_unsetstatus (p, NODE_STAT_THREAD);
	node_setstatus (p, NODE_STAT_DISCONNECT); 

	
}

void * rrlistener (void *param)
{
	int listenfd; 
	struct sockaddr_in svraddr;
	struct sockaddr_in cliaddr;
	int nOn = 1;
	int r;
	socklen_t clen;
	int csd;
	int nodeidx;
	PCNODE_PROP cluster_node;
	PCIRCULARLIST cluster_nodes;

	x_fprintf (stdout, "rrlistener");
	
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&svraddr, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(1101); 

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &nOn, sizeof(nOn));
	r = bind(listenfd, (struct sockaddr *) &svraddr, sizeof(svraddr));
	if (r != 0) 
	{
		x_fprintf(stderr, "bind: %d", errno);
		return 0;
	}

	r = listen(listenfd, 256);
	if (r != 0) 
	{
		x_fprintf(stderr, "error listen %s", strerror(errno));
		return 0;
	}

	while (1) 
	{
		clen = sizeof(cliaddr);
		bzero(&cliaddr, sizeof(cliaddr));
		csd = accept(listenfd, (struct sockaddr *) &cliaddr, &clen);
		if (csd < 0)
		{
			x_fprintf(stderr,  "%s", strerror(errno));
			continue; // accept connection failed
		}
		x_fprintf(stdout,  "new connection accepted. fd = %d", csd);

		// assign socket
		cluster_node = calloc (1, sizeof (CNODE_PROP)); 
		cluster_node->nodesd = csd;
		node_setstatus(cluster_node,  NODE_STAT_CONNECTION);
		x_fprintf(stdout,  "node is assigned id = %d", nodeidx);
		cluster_node->id = 0;
		cluster_nodes->add (cluster_nodes, 
			newcircularitem("nex",cluster_node, sizeof (*cluster_node)));

		// initialize thread
		pthread_mutex_init(&(cluster_node->mx_fd), NULL);
		pthread_create(&(cluster_node->node_tid), &t_node_attr,
		    node_handler, (cluster_node));
		node_setstatus(cluster_node, NODE_STAT_THREAD);
	} 
}

int main (int argc, char **argv)
{

}
