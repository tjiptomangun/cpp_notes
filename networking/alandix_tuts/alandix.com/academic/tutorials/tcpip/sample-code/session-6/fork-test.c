/****************************************/
/*                                      */
/*      fork-test.c                     */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      29th February 1996              */
/*                                      */
/****************************************/

#include <stdio.h>
#include  <sys/types.h>
#include  <sys/time.h>
#include  <errno.h>
#include  <signal.h>


main(int argc, char **argv)
{
    pid_t  parent_pid;
    pid_t  child_pid;
    pid_t  my_pid;

    parent_pid =  getpid();
    printf("process %d about to fork\n",parent_pid);
    child_pid  =  fork();
    my_pid     =  getpid();
    if ( child_pid == 0 ) {  /* child */
        printf("I am %d the child\n",my_pid);
        }
    else if ( child_pid > 0 ) {  /* parent */
        printf("I am %d the parent of %d\n",my_pid,child_pid);
        }
    else  {  /* error on fork */
        printf("I am %d the fork failed with %d\n",my_pid,child_pid);
        }
    
}

