/**
 * kill -USR1 pid
 * kill -USR2 pid
 * kill pid
 */

#include "../apue.h"

static void sig_usr(int signo);

int main(void) {
	if (signal(SIGUSR1, sig_usr)  == SIG_ERR) {
		err_sys("can't catch SIGURSR1");
	} else if (signal(SIGUSR2, sig_usr)  == SIG_ERR) {
		err_sys("can't catch SIGURSR1");
	} else {
		for (; ;) {
			pause();
		}
	}
}

static void sig_usr(int signo) {
	if (signo == SIGUSR1) {
		printf("received SIGUSR1\n");
	}
	else if (signo == SIGUSR2) {
		printf("received SIGUSR2\n");
	} else {
		err_dump("received signal %d\n", signo);
	}
}
