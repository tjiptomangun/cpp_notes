/**
 * $CC -Wall -ggdb3 mempoolmutex.c -lpthread -o mempoolmutex
 * http://cboard.cprogramming.com/linux-programming/112623-mutex-shared-memory-segment-questions.html
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#define __USE_UNIX98
#include <pthread.h>
 
#define PROG1 "This is program 1\n"
#define PROG2 "This is program 2\n"
 
static char * myLine = PROG1;
static int shmid;
 
pthread_mutex_t * getSharedMutexPtr(void)
{
	key_t key = 0x11d7;
	FILE * fp; 
	shmid = shmget(key, sizeof(pthread_mutex_t), IPC_CREAT | IPC_EXCL | 0x1b6); 
	if (0 > shmid)
	{
		printf("Could not create shared mem.\n");
		printf("Will attempt to find it.\n");
			
		shmid = shmget(key, sizeof(pthread_mutex_t), 0x1b6);
			 
		if(0 > shmid)
		{
			printf("\tCouldnt find it either\n");
		}
		else
		{
			 
			fp = fopen("MutextestFile.txt", "r+");
			if (fp)
				fclose(fp);  
			else
				perror ("[ER] MutextFile.txt");

			 
			myLine = PROG2;
			printf("\tFound shared memory");
			void * const poSharedMem = shmat(shmid, NULL, 0);
			 
			if(((void *)-1) == poSharedMem)
			{
			    printf("Could not attatch shared memory to address space.\n");
			    return (pthread_mutex_t *) NULL;
			}
			else
			{
			    //THIS IS WHERE I WOULD LIKE TO MARK MEMORY FOR DESTRUCTION
			    //printf("Shared memory attached and marked for deletion.\n");
			    //shmctl(shmid, IPC_RMID, NULL);
			    printf("Shared memory attached\n");
			    return (pthread_mutex_t *) poSharedMem;       
			}
		}
	}
	else
	{
		fp = fopen("MutextestFile.txt", "w+");
		fclose(fp);
 
		myLine = PROG1;
		printf("Shared memory created.\n");
		 
		void * const poSharedMem = shmat(shmid, NULL, 0);
		 
		if(((void *)-1) == poSharedMem)
		{
			printf("Could not attatch shared memory to address space.\n");
			return (pthread_mutex_t *) NULL;
		}
		else
		{
		//THIS IS WHERE I WOULD LIKE TO MARK MEMORY FOR DESTRUCTION
		//printf("Shared memory attached and marked for deletion.\n");
		//shmctl(shmid, IPC_RMID, NULL);
			printf("Shared memory attached\n");
			return (pthread_mutex_t *) poSharedMem;
		}
	}
 
	return (pthread_mutex_t *) NULL;
}
 
int detatchFromSharedMem(void * poSharedMem)
{
	//THIS IS WHERE I MARK MEMORY FOR DESTRUCTION CURRENTLY BUT NOT HAPPY WIHT IT
	printf("Marking shared memory for destruction and detaching from it.\n");
	shmctl(shmid, IPC_RMID, NULL);
	return shmdt(poSharedMem);
}
 
pthread_mutex_t * sharedMemoryMutexInit(void)
{
	pthread_mutex_t * pMutex = getSharedMutexPtr(); 
	pthread_mutexattr_t oMutexAttribute; 

	if (NULL == pMutex)
		return (pthread_mutex_t *) NULL;
	pthread_mutexattr_init(&oMutexAttribute); 
	if (0 != pthread_mutexattr_setpshared(&oMutexAttribute, PTHREAD_PROCESS_SHARED)) 
	{
		pthread_mutexattr_destroy(&oMutexAttribute);
		return (pthread_mutex_t *) NULL;
	}
	if (0 != pthread_mutexattr_settype(&oMutexAttribute, PTHREAD_MUTEX_RECURSIVE))
	{
		pthread_mutexattr_destroy(&oMutexAttribute);
		return (pthread_mutex_t *) NULL; 
	}
	if (0 != pthread_mutex_init(pMutex, &oMutexAttribute))
	{
		pthread_mutexattr_destroy(&oMutexAttribute);
		return (pthread_mutex_t *) NULL; 
	}
	printf("Mutex initialized \n");
	return pMutex; 
}
void usage (char * appname)
{
	fprintf (stderr, "usage %s procname\n", appname);
} 
int main(int argc, char **argv)
{
	pthread_mutex_t * GlobalMutex;
	FILE * fp;
	int i = 0;
	if (argc < 2)
	{
		usage(argv[0]);
		exit (EXIT_FAILURE);
	}
 
	GlobalMutex = sharedMemoryMutexInit();
	 
	if (GlobalMutex == NULL)
	{
		return 0;
	}
	 
	for (i =0; i < 100000; i ++)
	{
		if (0 == pthread_mutex_lock(GlobalMutex))
		{
			fp = fopen("MutextestFile.txt", "r+");
			if (NULL != fp)
			{
			    fseek(fp, 0, SEEK_END);
			    fprintf(fp, "%4d - ", i+1);
			    fprintf(fp, "%s\n", argv[1]);
			    fclose(fp);
			    fp = NULL;
			}
			pthread_mutex_unlock(GlobalMutex);
		}
	}
	 
	 
	if(NULL != GlobalMutex)
	{
		detatchFromSharedMem(GlobalMutex);
	}
	return 0;
}
 
//EOF
