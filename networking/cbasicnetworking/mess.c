/****************************************/
/*                                      */
/*      mess.c                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      print to tty and transcript     */
/*                                      */
/*      8th January 1990                */
/*                                      */
/****************************************/

#include  <stdio.h>
#include <stdarg.h>
#include "mess.h"

FILE       *fp = NULL;

void  init_mess(char *transcript)
{
    fp = fopen(transcript,"w");
    if ( fp == NULL ) perror("opening transcript");
}

void  mess(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr,fmt,args);
    fflush(stderr);
    if ( fp ) {
        vfprintf(fp,fmt,args);
        fflush(fp);
        }

}
