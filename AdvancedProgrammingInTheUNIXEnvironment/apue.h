#ifndef _APUE_H_
#define _APUE_H_
#define _XOPEN_SOURCE_	600 /*Single UNIX Specification, Version 3*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/*
* Prototypes for our own functions.
*/

void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_exit(int, const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);
#endif
