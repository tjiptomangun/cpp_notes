#include <stdio.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ctlv.h"

int cont = 1;
void alarmHandler(int signum)
{
	cont = 0;
}
int j = 0;
typedef struct dataitem
{
	unsigned char *data;
	int datalen ;
	struct dataitem *next;
}DATAITEM, *PDATAITEM;

typedef struct datalist
{
	PDATAITEM head; 
}DATALIST, *PDATALIST;

PDATAITEM newdataitem (char *strin)
{
	int nlen = strlen (strin); 
	PDATAITEM item = NULL;
	unsigned char *data ;
	item = (PDATAITEM) calloc (1, sizeof (DATAITEM));
	if (!item)	
		return NULL;
	data = (unsigned char *) calloc (1, nlen/2);
	if (!data) 
		return 0;
	HtoB (data, strin, nlen);
	item->data = data;
	item->datalen = nlen/2;
	item->next = NULL;
		
	return item;
}
PDATALIST newdatalist ()
{
	PDATALIST ret = (PDATALIST) calloc (1, sizeof (DATALIST));
	return ret;
}
int datalist_add (PDATALIST list, PDATAITEM item)
{
	PDATAITEM curr;
	if (list->head == NULL)
	{
		list->head = item;
		return 0;
	}
	curr = list->head;
	while (curr->next)
	{
		curr = curr->next;
	}
	curr->next = item;

	return 0;
}

PDATAITEM  datalist_gethead(PDATALIST list)
{
	PDATAITEM ret = NULL;
	if(!list->head)
		return NULL;
	ret = list->head;
	list->head = list->head->next;
	return ret;
}

PDATAITEM datalist_gettail(PDATALIST list)
{
	PDATAITEM ret = NULL, prev = NULL; 
	if (!list->head)
		return NULL;
	ret = list->head;
	while (ret)
	{ 
		prev = ret;	
		ret = ret->next;
	}
	if(prev == list->head)
		list->head=NULL;
	return prev;
}
#define MAX_PROCESSED_DATA 2048

int main (int argc, char **argv)
{ 
	int i = 0; 
	PDATALIST datalist= newdatalist (); 
	PDATAITEM item;
	PTLVPARSER p;
	int datalen = 0;
	unsigned char data[MAX_PROCESSED_DATA];
	char dataStr[MAX_PROCESSED_DATA * 2];
	int liststest = 0;
	int listvtest = 0;
	int listrtest = 0;
	int treestest = 0;
	int treevtest = 0;
	int treertest = 0;
	if (argc < 2)
	{
		fprintf (stderr, "usage %s hexbin data ....\n", argv[0]);
		exit (EXIT_FAILURE); 
	} 
	if (argc > 2)
	{
		if (!strcmp (argv[1], "-s"))
		{
			liststest = 1;
			listvtest= 0;
			treestest = 0;
			treevtest = 0;
			listrtest = 0;
			treertest = 0;
		}
		else if (!strcmp (argv[1], "-v"))
		{
			listvtest = 1;
			liststest = 0;
			treestest = 0;
			treevtest = 0;
			listrtest = 0;
			treertest = 0;
		}
		else if (!strcmp (argv[1], "-V"))
		{
			listvtest = 0;
			liststest = 0;
			treestest = 0;
			treevtest = 1;
			listrtest = 0;
			treertest = 0;
		}
		else if (!strcmp (argv[1], "-S"))
		{
			listvtest = 0;
			liststest = 0;
			treestest = 1;
			treevtest = 0;
			listrtest = 0;
			treertest = 0;
		}
		else if (!strcmp (argv[1], "-R"))
		{
			listvtest = 0;
			liststest = 0;
			treestest = 0;
			treevtest = 0;
			listrtest = 0;
			treertest = 1;
		}
		else if (!strcmp (argv[1], "-r"))
		{
			listvtest = 0;
			liststest = 0;
			treestest = 0;
			treevtest = 0;
			listrtest = 1;
			treertest = 0;
		}

	}
	for (i=0; i<argc-(1+liststest+listvtest+treestest+treevtest+listrtest+treertest); i++)
	{
		datalist_add(datalist, newdataitem (argv[i+1+liststest+
			listvtest+treevtest+treestest+listrtest+treertest]));
	}
	if (listvtest)
	{
		p = newtlvparser((PTREECOMPONENT)newroottlvtreeitem());
		item = datalist->head;
		while (item)
		{
			p->processStream(p,item->data , item->datalen);
			item = item->next;
		} 
		datalen = p->root->serializetlv (p->root, data, MAX_PROCESSED_DATA);
		if (datalen>0)
		{
			memset(dataStr, 0, MAX_PROCESSED_DATA * 2);
			BtoH(dataStr, data, datalen);
			fprintf (stdout, "%s", dataStr);
			fprintf (stdout,"\n");
		}	
		exit (EXIT_SUCCESS);
	}
	else if (liststest || listrtest)
	{
		p = newtlvparser((PTREECOMPONENT)newroottlvtreeitem());
		if (liststest)
		{
			signal (SIGALRM, alarmHandler);
			alarm (1); 
		}
		do	
		{
			item = datalist->head;
			while (item)
			{
				p->processStream(p,item->data , item->datalen);
				item = item->next;
			} 
			datalen = p->root->serializetlv ((PTREECOMPONENT)&p->root, data, MAX_PROCESSED_DATA);
			p->root->initialize(p->root,NULL, NULL); 
			j++;
		}
		while (cont); 
		fprintf (stdout ,"\nnum iter = %d\n", j);
		if (liststest);
			alarm (0); 

	}
	else if (treevtest)
	{
		p = newtlvparser((PTREECOMPONENT)newroottlvtreenode());
		item = datalist->head;
		while (item)
		{
			p->processStream(p,item->data , item->datalen);
			item = item->next;
		} 
		datalen = p->root->serializetlv (p->root, data, MAX_PROCESSED_DATA);
		if (datalen>0)
		{
			memset(dataStr, 0, MAX_PROCESSED_DATA * 2);
			BtoH(dataStr, data, datalen);
			fprintf (stdout, "%s", dataStr);
			fprintf (stdout,"\n");
		} 
		p->root->initialize(p->root,NULL, NULL); 
		exit (EXIT_SUCCESS);


	}
	else if (treestest || treertest)
	{
		p = newtlvparser((PTREECOMPONENT)newroottlvtreenode());
		if (treestest)
		{
			signal (SIGALRM, alarmHandler);
			alarm (1); 
		}
		do	
		{
			item = datalist->head;
			while (item)
			{
				p->processStream(p,item->data , item->datalen);
				item = item->next;
			} 
			datalen = p->root->serializetlv ((PTREECOMPONENT)&p->root, data, MAX_PROCESSED_DATA);
			p->root->initialize(p->root,NULL, NULL); 
			j++;
		}
		while (cont); 
		fprintf (stdout ,"\nnum iter = %d\n", j);
		if (treestest)
			alarm (1); 

	}
	//fprintf (stdout ,"\nnum child = %d\n", p->root.count);
	exit (EXIT_SUCCESS);
}
