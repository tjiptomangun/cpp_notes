#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
/**
 * $CC -Wall -ggdb3 timercreat.c -o timercreat 
 **/

#define TRUE 1
int bXALAlertTimerFlag = 0;
void XALAlertTimerSignal (int nSigNo)
{
	bXALAlertTimerFlag = TRUE;

} /* End of XALTimerSignal */

int main (int argc, char **argv)
{
	timer_t timerid;
	int 			nStatus;
//	struct itimerspec	TimerSpec;
	struct sigevent		SigEvent;
	struct sigaction 	SigAct;

	/* Set up signal handler: */
	sigfillset (&SigAct.sa_mask);
	SigAct.sa_flags		= 0;
	SigAct.sa_handler	= XALAlertTimerSignal;
	sigaction (SIGALRM, &SigAct, NULL);

	/* Set up timer: */
	memset (&SigEvent, 0, sizeof (SigEvent));
	SigEvent.sigev_notify                       = 0; /*SIGEV_SIGNAL*/
	SigEvent.sigev_signo			= SIGALRM;
	SigEvent.sigev_value.sival_int	= 0;

	nStatus = timer_create (CLOCK_REALTIME, &SigEvent, &timerid);


	fprintf (stdout, "status = %d timerid = %d \n", nStatus, timerid);

	return 0;
	


}
