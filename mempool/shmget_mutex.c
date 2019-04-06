/**
 * http://cboard.cprogramming.com/linux-programming/112623-mutex-shared-memory-segment-questions.html
 * $CC shmget_mutex.c -Wall -ggbd3 -lpthread -o shmget_mutex
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#define __USE_GNU
#define __USE_UNIX98
#include <pthread.h> 

#define PROG1 "This is program 1\n"
#define PROG2 "This is program 2\n"

static pthread_mutex_t * GlobalMutex;
static char * myLine = PROG1;
static int shmid;
void sharedMemoryMutexInit(void * poSharedMem)
{
    pthread_mutex_t * GlobalMutex = (pthread_mutex_t *) poSharedMem;
    pthread_mutexattr_t oMutexAttribute;
    pthread_mutexattr_init(&oMutexAttribute);
    if(0 == pthread_mutexattr_setrobust_np(&oMutexAttribute, PTHREAD_MUTEX_ROBUST_NP))
    {
        if(0 == pthread_mutexattr_setpshared(&oMutexAttribute, PTHREAD_PROCESS_SHARED))
        {
            if(0 == pthread_mutexattr_settype(&oMutexAttribute, PTHREAD_MUTEX_ERRORCHECK))
            {
                if(0 == pthread_mutex_init(GlobalMutex, &oMutexAttribute))
                {
                    printf("GlobalMutex initialized.\n");
                }
                else
                {
                    printf("pthread_mutex_init Failure.\n");
                }
            }
            else
            {
                printf("pthread_mutexattr_settype Failure.\n");
            }
        }
        else
        {
            printf("pthread_mutexattr_setpshared Failure.\n");
        }
    }
    else
    {
        printf("pthread_mutexattr_setrobust_np Failure.\n");
    }
    pthread_mutexattr_destroy(&oMutexAttribute);
}
#define  bool unsigned char
void * getSharedMemPtr(key_t key, size_t size, bool mutex)
{
    int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0x1b6);
    if(-1 == shmid)
    {
        //printf("Could not create shared mem.\nWill attempt to find it.\n");
        shmid = shmget(key, size, IPC_CREAT | 0x1b6);
        if(-1 == shmid)
        {
            printf("\tCould not find it either.\n");
        }
        else
        {
            //printf("\tFound shared memory.\n");
            void * poSharedMem = shmat(shmid, 0, 0);
            if((void *) -1 == poSharedMem)
            {
                printf("Could not attatch shared memory to address space.\n");
            }
            else
            {
                //printf("Shared memory attached.\n");
                return poSharedMem;
            }
        }
    }
    else
    {
        printf("Shared memory created.\n");
        void * poSharedMem = shmat(shmid, 0, 0);
        if((void *) -1 == poSharedMem)
        {
            printf("Could not attatch shared memory to address space.\n");
        }
        else
        {
            //printf("Shared memory attached.\n");
            if(mutex)
            {
                sharedMemoryMutexInit(poSharedMem);
            }
            return poSharedMem;
        }
    }
    return 0;
}
int detatchFromSharedMem(void * poSharedMem)
{
  printf("Marking shared memory for destruction and detaching from it.\n");
  shmctl(shmid, IPC_RMID, NULL);
  return shmdt(poSharedMem);
}
 

#define true 1
int main(void)
{
 
  FILE * fp;
  key_t key = 0x11d7;
  int i = 0;
  
  void * poSharedMem = getSharedMemPtr(key, sizeof(pthread_mutex_t) + sizeof(pthread_once_t), 1);
  GlobalMutex = (pthread_mutex_t *)poSharedMem;
 
  pthread_once_t * pOnce = (pthread_once_t *)( ((char *)poSharedMem) + sizeof(pthread_mutex_t) );
 
  pthread_once(pOnce, sharedMemoryMutexInit);
   
  if (GlobalMutex == NULL)
    {
      return 0;
    }
 
  int mutexLockResult;
   
  for (i =0; i < 100000; i ++)
    {
      mutexLockResult = pthread_mutex_lock(GlobalMutex);
      if (0 == mutexLockResult)
    {
      fp = fopen("MutextestFile.txt", "r+");
      if (NULL != fp)
        {
          fseek(fp, 0, SEEK_END);
          fprintf(fp, "%4d - ", i+1);
          fprintf(fp, myLine);
          fclose(fp);
          fp = NULL;
        }
      pthread_mutex_unlock(GlobalMutex);
    }
      else if (EOWNERDEAD == mutexLockResult)
    {
      fp = fopen("MutextestFile.txt", "r+");
      if (NULL != fp)
        {
          fseek(fp, 0, SEEK_END);
          fprintf(fp, "%4d - ", i+1);
          fprintf(fp, myLine);
          fclose(fp);
          fp = NULL;
        }
      pthread_mutex_consistent_np(GlobalMutex);
      pthread_mutex_unlock(GlobalMutex);
    }
    }
   
   
  if(NULL != GlobalMutex)
    {
      detatchFromSharedMem(GlobalMutex);
    }
}
 
//EOF
