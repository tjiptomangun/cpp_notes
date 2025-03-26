//$CC -Wall -ggdb3 -lpthread threadtest.c -o threadtest
//want to prove if (m|c)alloc return the same address
//it is true
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
typedef struct llnode
{
	pthread_t *ptid;
	struct llnode *next;
}LLNODE , *PLLNODE;

PLLNODE newllnode (pthread_t *ptid)
{
	PLLNODE nn = (PLLNODE) calloc (1, sizeof (LLNODE));
	nn->ptid = ptid;
	nn->next = NULL;
	return nn;
}
PLLNODE head = NULL;
pthread_mutex_t llmx = PTHREAD_MUTEX_INITIALIZER;
void attach_llnode (PLLNODE new)
{
	PLLNODE curr;
	pthread_mutex_lock (&llmx);
	if (!head)
	{
		head = new;
	}
	else
	{
		curr = head;
		while (curr)
		{
			if (curr->next == NULL)
			{
				curr->next = new;
				break;
			}	
			curr = curr->next;
		}
	}
	pthread_mutex_unlock (&llmx);
}
/**
 * detach a node
 **/
PLLNODE detach_node (PLLNODE det)
{
	PLLNODE curr, prev = NULL;
	curr = head;
	PLLNODE ret = NULL;
	pthread_mutex_lock (&llmx);
	while (curr)
	{
		if (curr == det)
		{
			if (prev)
				prev->next = det;
			else
				head = curr->next;
			ret = curr;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	pthread_mutex_unlock (&llmx);
	return ret;
}
PLLNODE find_tid (pthread_t tid)
{
	PLLNODE curr, ret = NULL;
	curr = head;
	pthread_mutex_lock (&llmx);
	while (curr)
	{
		if (*curr->ptid == tid)
		{
			ret = curr;
			break;
		}
		curr = curr->next;
	}
	pthread_mutex_unlock (&llmx);
	return ret; 
	
}
/**
 * find a tid
 **/
PLLNODE find_ptid(pthread_t *ptid)
{
	PLLNODE curr, ret = NULL;
	curr = head;
	pthread_mutex_lock (&llmx);
	while (curr)
	{
		if (curr->ptid == ptid)
		{
			ret = curr;
			break;
		}
		curr = curr->next;
	}
	pthread_mutex_unlock (&llmx);
	return ret; 
}
int delete_node (pthread_t *ptid)
{
	PLLNODE find = find_ptid (ptid);
	int ret = -1;
	if (find && (detach_node (find)==find))
	{
		free(find);
		ret = 0;
	}
	return ret;
}
int add_ptid (pthread_t *ptid)
{
	PLLNODE newll = newllnode (ptid);
	attach_llnode (newll);
	return 0;
}
void * dummythread (void *dat)
{
	int i = 0;
	for (i = 0; i <2; i++)
	{
		sleep (2);
		fprintf (stdout, "%lX: dummy\n", pthread_self());
	}
	PLLNODE k = detach_node(find_tid(pthread_self()));
	fprintf (stdout, "%lX : end\n", pthread_self());
	free (k->ptid);
	free (k);
	pthread_exit (NULL);
}

int main (int argc, char **argv)
{
	pthread_t *tid ;
	int r ;
	while (1)
	{
		tid = calloc(1, sizeof (pthread_t));
		r = pthread_create (tid, 0, dummythread, NULL);
		pthread_detach (*tid); 
		add_ptid (tid); 
		sleep (3);
	}	
}
