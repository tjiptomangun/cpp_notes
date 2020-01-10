/**
 * gcc -m32 parserclass.c ctlvtest.c -Wall -ggdb3 -lctlv -o ctlvtest 
 **/
#include <stdio.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ctlv.h"
#include "parserclass.h"

/**
 * Name		: newctlvbtree
 * Description	: create new cxtlv that uses hierachical binary tress structure to store tlv
 **/
extern PCXTLV newcxtlvbtree();


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
int isNumber (char *k)
{
	int len= strlen(k);
	int i = 0;

	for (i = 0; i < len; i++)
	{
		if (!isdigit (k[i]))
			return 0;
	}
	return 1;
}
int isHexNumber (char *ks)
{
	int len = strlen (ks);
	char k;
	int i = 0;
	for (i = 0; i < len; i++)
	{
		k = ks[i];
		if ((!(k>='0' && k<='9')) &&
			(!(k>='A' && k<='F')) &&
			(!(k>='a' && k<='f')))
		{
			return 0;
		}
	}
	return 1;
}
int option ()
{
	fprintf (stdout, "type:\n"
			"get item_name - to set active item\n"
			"print - print properties of active item\n"
			"q - quit application\n"); 
	return 0;
}
#define MAX_STACKPTR 1000
#define TOKEN_SPACE 0
#define TOKEN_CR 1
#define TOKEN_PARAM 2
#define TOKEN_KEYWORD 3
int stdparse (PCXTLV p, FILE *fp)
{
	char tokenlist[] = {" \n"};
	char buffer[MAX_PROCESSED_DATA];
	unsigned char data[MAX_PROCESSED_DATA];
	char str_data[MAX_PROCESSED_DATA];
	int token = -1;
	int prevtoken = -1;
	int length = 0;
	int ret;

	int offset = 0;
	int datalen = 0;
	int returnlen = 0;

	int state = 0;
	unsigned char * newvar;
	unsigned char tagbuff[MAX_TAGSTRING_LEN];
	PSTACK_PTR stack = newstackptr();

#define STATE_ADD 	0x01
#define STATE_DELETE	0x02
#define STATE_GET	0x03
#define STATE_UPDATE	0x04 
#define STATE_OTHER	0x00
	option();
	token = stream_gettoken (fp, tokenlist, buffer, MAX_PROCESSED_DATA, &length);
	state = STATE_OTHER;
	while (token != -1)
	{ 
		switch (tokenlist[token])
		{
		case ' ':
			trimstring (buffer);
			if (!strlen (buffer))
				break;
			else if (!strcmp (buffer, "q"))
			{
				fprintf (stdout, "bye...\n");
				return 0;
			}
			else if (!strncmp (buffer, "get", 3)) 
			{
				state = STATE_GET; 
			}
			else if (!strncmp (buffer, "add", 3))
			{
				state = STATE_ADD;
			}
			else if (!strncmp (buffer, "update", 6))
			{
				state = STATE_UPDATE;
			}
			else if (!strncmp (buffer, "delete", 6)) 
			{
				state = STATE_DELETE; 
			}
			else 
			{ 
				newvar = (unsigned char *)  calloc (length +1, 1);
				if (!newvar)
				{
					fprintf (stderr, "out of memory\n");
					continue;
				} 
				memcpy (newvar, buffer, length);
				if (stack->push (stack, newvar) )
				{
					fprintf (stderr, "stack overflow\n");
					free (newvar);
				} 
			} 
			break;
		case '\n':
			offset = 0;
			datalen = 0;
			returnlen = 0;
			trimstring (buffer);
			memset (tagbuff, 0, MAX_TAGSTRING_LEN);
			if (!strcmp (buffer, "q"))
			{
				fprintf (stdout, "bye...\n");
				return 0;
			}
			if (strlen (buffer))
			{
				newvar = (unsigned char *)  calloc (length +1, 1);
				if (!newvar)
				{
					fprintf (stderr, "out of memory\n");
					continue;
				} 
				memcpy (newvar, buffer, length);
				if (stack->push (stack, newvar) )
				{
					fprintf (stderr, "stack overflow\n");
					free (newvar);
				} 
			}
			switch (state)
			{
			case STATE_DELETE:/*delete specified tags*/
				if (stack->top<0)
				{
					fprintf (stderr, "[ERR] - delete tag\n");
					break;
				}
				HtoB (tagbuff, (char *)stack->c[0], strlen ((char *)stack->c[0]));
				ret = p->Delete (p, tagbuff); 
				break;
			case STATE_GET:/*get treecomponent with specified tag, you can define offset and length*/
				memset (data, 0 , MAX_PROCESSED_DATA);
				memset (str_data, 0 , MAX_PROCESSED_DATA);
				if (stack->top<1)
				{
					fprintf (stderr, "[ERR] - get tag [offset] length \n");
					break;
				}
				HtoB (tagbuff, (char *)stack->c[0], strlen ((char *)stack->c[0]));
				if (stack->top==1) 
				{
					datalen= atoi ((char*)stack->c[1]);
				}
				if (stack->top==2)
				{
					datalen = atoi ((char*)stack->c[2]); 
					offset = atoi ((char*)stack->c[1]); 
				}
				returnlen = datalen;
				if (stack->top <= 1)
					ret = p->GetDataFixedL(p, tagbuff, data,datalen);
				else
					ret = p->GetDataVarL(p, tagbuff, data, offset, datalen, &returnlen);
				BtoH (str_data, data, returnlen);	
				fprintf (stdout, "function retval = %d\n", ret);
				fprintf (stdout, str_data);
				fprintf (stdout, "\n");
				
				break;
			case STATE_UPDATE:/*update specified tag values, you can define offset. values suplied is hex*/
				memset (data, 0 , MAX_PROCESSED_DATA);
				if (stack->top <1)
				{
					fprintf (stderr, "[ERR] - update tag data [offset] [len]");
					break;
				}
				HtoB (tagbuff, (char *)stack->c[0], strlen ((char *)stack->c[0]));
				HtoB (data, (char *)stack->c[1], strlen ((char *)stack->c[1]));
				datalen = strlen (stack->c[1])>>1;
				if (stack->top >=2)
					offset  = atoi (stack->c[2]);
				if (stack->top >=3)
					datalen = atoi (stack->c[3]);	
				if (stack->top >=2) 
					returnlen = p->SetDataVarL (p,tagbuff, data, offset, datalen);
				else
					returnlen = p->SetDataFixedL (p, tagbuff, data, datalen);
				fprintf (stdout, "result:%d\n", returnlen); 
				
				break;
			case STATE_OTHER:/*gggrrrrrmmmppphhhh*/
				break;
			default:
				break;
			} 
			stack->cleanup(stack);
			state = STATE_OTHER; 
			break;
		default:
			break;

		}
		prevtoken = token;
		memset (buffer, 0, length);
		token = stream_gettoken (fp, tokenlist, buffer, MAX_PROCESSED_DATA, &length);
	}
	return 0;

}
int usage(char *appName, FILE *dest)
{
	fprintf (dest, "usage %s -[svriSVRI] hexbin data ....\n", appName);
	fprintf (dest, " %s is tool to test ctlv library (libctlv.so).\n",appName);
	fprintf (dest, " libctlv.so has two  tree structure that can be used to store tlv data.\n");
	fprintf (dest, " the first is hierarchical linked list\n");
	fprintf (dest, " the second  is hierarcical binary tree (default)\n");
	fprintf (dest, " these to options are deliberately provided to enable us to compare\n");
	fprintf (dest, " algorithm performance.\n");
	fprintf (dest, " all upper case  option will use hierarchical linked list.\n"); 
	fprintf (dest, " all lower case  option will use hierarchical binary tree.\n"); 
	fprintf (dest, "v/V verbose test , it will read hexbin data and construct tree (deserializing).\n");
	fprintf (dest, "  it will then serialize data to stream and print as hexbin data.\n");
	fprintf (dest, "s/S performance test , it will count how many deserialize , serialize , reset\n");
	fprintf (dest, "  tree performed in 1 second.\n");
	fprintf (dest, "r/R resource test, it will deserialize stream, serialize tree, reset tree \n");
	fprintf (dest, "  infinitely. you can check resource used using top(linux) or topas(aix)\n");
	fprintf (dest, "  watch the memory usage\n");
	fprintf (dest, "i/I interactive test, it will enable you to update and get data in an interactive\n");
	fprintf (dest, "  mode\n"); 
	return 0;

}
int main (int argc, char **argv)
{ 
	int i = 0; 
	PDATALIST datalist= newdatalist (); 
	PDATAITEM item;
	PCXTLV p;
	int datalen = 0;
	unsigned char data[MAX_PROCESSED_DATA];
	char dataStr[MAX_PROCESSED_DATA * 2];
	int lstest = 0;
	int lvtest = 0;
	int lrtest = 0;
	int litest = 0;
	int tstest = 0;
	int tvtest = 0;
	int trtest = 0;
	int titest = 0;
	if (argc <= 2 )
	{
		if (argc == 2)
		{
		
			if (!strcmp (argv[1], "-h"))
			{
				usage (argv[0], stdout);
				exit (EXIT_SUCCESS); 
			}
		}
		usage (argv[0], stderr);	
		exit (EXIT_FAILURE); 
	} 
	if (argc > 2)
	{
		if (!strcmp (argv[1], "-s"))
		{
			lstest = 1;
		}
		else if (!strcmp (argv[1], "-v"))
		{
			lvtest = 1;
		}
		else if (!strcmp (argv[1], "-V"))
		{
			tvtest = 1;
		}
		else if (!strcmp (argv[1], "-S"))
		{
			tstest = 1;
		}
		else if (!strcmp (argv[1], "-R"))
		{
			trtest = 1;
		}
		else if (!strcmp (argv[1], "-r"))
		{
			lrtest = 1; 
		}
		else if (!strcmp (argv[1], "-i"))
		{
			litest = 1; 
		}
		else if (!strcmp (argv[1], "-I"))
		{
			titest = 1; 
		}

	}
	for (i=0; i<argc-(1+lstest+lvtest+tstest+tvtest+lrtest+trtest+litest+titest); i++)
	{
		datalist_add(datalist, newdataitem (argv[i+1+lstest+
			lvtest+tvtest+tstest+lrtest+trtest+litest+titest]));
	}
	if (lvtest)
	{
		p = NewDefaultCXTlv();
		item = datalist->head;
		while (item)
		{
			p->Deserialize(p,item->data , item->datalen);
			item = item->next;
		} 
		datalen = p->Serialize (p, data, MAX_PROCESSED_DATA);
		if (datalen>0)
		{
			memset(dataStr, 0, MAX_PROCESSED_DATA * 2);
			BtoH(dataStr, data, datalen);
			fprintf (stdout, "%s", dataStr);
			fprintf (stdout,"\n");
		}	
		exit (EXIT_SUCCESS);
	}
	else if (lstest || lrtest)
	{
		p = NewDefaultCXTlv();
		if (lstest)
		{
			signal (SIGALRM, alarmHandler);
			alarm (1); 
		}
		do
		{
			item = datalist->head;
			while (item)
			{
				p->Deserialize(p,item->data , item->datalen);
				item = item->next;
			} 
			datalen = p->Serialize (p, data, MAX_PROCESSED_DATA);
			p->Cleanup(p); 
			j++;
		}
		while (cont); 
		fprintf (stdout ,"\nnum iter = %d\n", j);
		if (lstest)
			alarm (0); 

	}
	else if (litest || titest)
	{
		if (litest)
			p = NewDefaultCXTlv(); 
		else
			p = newcxtlvbtree(); 
		item = datalist->head;
		while (item)
		{
			p->Deserialize(p,item->data , item->datalen);
			item = item->next;
		} 
		stdparse (p, stdin);
	}
	else if (tvtest)
	{
		p = newcxtlvbtree();
		item = datalist->head;
		while (item)
		{
			p->Deserialize(p, item->data, item->datalen);
			item = item->next;
		} 
		datalen = p->Serialize(p, data, MAX_PROCESSED_DATA);
		if (datalen>0)
		{
			memset(dataStr, 0, MAX_PROCESSED_DATA * 2);
			BtoH(dataStr, data, datalen);
			fprintf (stdout, "%s", dataStr);
			fprintf (stdout,"\n");
		} 
		p->Cleanup(p); 
		exit (EXIT_SUCCESS);


	}
	else if (tstest || trtest)
	{
		p = newcxtlvbtree();
		if (tstest)
		{
			signal (SIGALRM, alarmHandler);
			alarm (1); 
		}
		do	
		{
			item = datalist->head;
			while (item)
			{
				p->Deserialize(p,item->data , item->datalen);
				item = item->next;
			} 
			datalen = p->Serialize(p, data, MAX_PROCESSED_DATA);
			p->Cleanup(p); 
			j++;
		}
		while (cont); 
		fprintf (stdout ,"\nnum iter = %d\n", j);
		if (tstest)
			alarm (1); 

	}

	exit (EXIT_SUCCESS);
} 
