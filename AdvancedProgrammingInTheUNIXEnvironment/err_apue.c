#include "apue.h"
#include <errno.h>
#include <stdarg.h>

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap){
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(error));
	strcat(buf, "\n");
	fflush(stdout); /*in case stdout and stderr are the same*/
	fputs(buf, stderr);
	fflush(NULL); /*flushes all stdio output streams*/
}


/**
 * Nofatal error related to a system call.
 * Print a message and return
 */

void err_ret(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

/**
 * Fatal error related to a system call.
 * Print a message and terminate
 */
void err_sys(const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/**
 * Fatal error unrelated to a system call.
 * Error code passed as explicit parameter.
 * Print a message and terminate
 */
void err_exit(int error, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);	
	exit(1);
}

/**
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate
 */
void err_dump(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	abort();/* dump core and terminate */
	exit(1); /* shoudln't get here */ 
}

/**
 * Nofatal error unrelated to a system call.
 **/
void err_msg(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap); 
}

void err_quit(const char*fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
}
