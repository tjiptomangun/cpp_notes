/*******************************************/
/*                                         */
/*      mail-helper.c                      */
/*                                         */
/*      Alan Dix                           */
/*                                         */
/*      helper functions for writing       */
/*      simple SMTP mail clients           */
/*                                         */
/*      22nd January 1996                  */
/*                                         */
/*******************************************/

#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "mail-helper.h"

static FILE *echo_fp = NULL;

#define BUFF_LEN 200
static char buff[BUFF_LEN];
static int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

FILE *do_mail_set_echo_fp(FILE *echo_fp);
int do_mail_init(int serv_fd);
int do_mail_from(int serv_fd, char *from);
int do_mail_to(int serv_fd, char *to);
int do_mail_data_fp(int serv_fd, FILE *user_fp);
int do_mail_data_buff(int serv_fd, char *buff, int len);
int do_mail_quit(int serv_fd);

/*******************************/
/****  do_mail_set_echo_fp  ****/
/*******************************/

FILE *do_mail_set_echo_fp(FILE *new_echo_fp)
{
    FILE *old_echo_fp = echo_fp;
    echo_fp = new_echo_fp;
    return old_echo_fp;
}

/************************/
/****  do_mail_init  ****/
/************************/

int do_mail_init(int serv_fd)
{
    int ret;
    char hostname[32];

  /*  server gives 'response' first  */
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */

    gethostname(hostname,32);
    sprintf(buff,"HELO %s\n",hostname);
    if (echo_fp)  fprintf(echo_fp,">> %s\n",buff);
    write(serv_fd,buff,strlen(buff));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */

    return 0;
}


/************************/
/****  do_mail_quit  ****/
/************************/

int do_mail_quit(int serv_fd)
{
    int ret;

    if (echo_fp)  fprintf(echo_fp,">> QUIT\n");
    write(serv_fd,"QUIT\n",strlen("QUIT\n"));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */
    return 0;
}


/************************/
/****  do_mail_from  ****/
/************************/

int do_mail_from(int serv_fd, char *from)
{
    int ret;

  /*  send sender  */
    sprintf(buff,"MAIL FROM:<%s>\n",from);
    if (echo_fp)  fprintf(echo_fp,">> %s\n",buff);
    write(serv_fd,buff,strlen(buff));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */
    return 0;
}



/**********************/
/****  do_mail_to  ****/
/**********************/

int do_mail_to(int serv_fd, char *to)
{
    int ret;

  /*  send recipient  */
    sprintf(buff,"RCPT TO:<%s>\n",to);
    if (echo_fp)  fprintf(echo_fp,">> %s\n",buff);
    write(serv_fd,buff,strlen(buff));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */
    return 0;
}


/***************************/
/****  do_mail_data_fp  ****/
/***************************/

int do_mail_data_fp(int serv_fd, FILE *user_fp)
{
    int ret;

  /*  prepare to send message  */
    if (echo_fp)  fprintf(echo_fp,">> DATA\n");
    write(serv_fd,"DATA\n",strlen("DATA\n"));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 300 || ret > 399 )
                return 1;   /* some form of error message */

  /*  copy recipient's message - stop on EOF  */
    printf("Please type your message, use type EOF (^D) to finish\n");
    for(;;) {
      /*  wait for line from user     */
        if ( fgets(buff,BUFF_LEN,user_fp) == NULL )
                break;                 /*  user typed end of file       */
        if ( buff[0] == '.' )
                write(serv_fd,".",1);  /*  double up initial full stop  */
        write(serv_fd,buff,strlen(buff));
    }
  /*  message now complete, send terminating full stop  */
    if (echo_fp)  fprintf(echo_fp,">> .\n");
    write(serv_fd,".\n",2);
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */
    return 0;
}


/*****************************/
/****  do_mail_data_buff  ****/
/*****************************/

int do_mail_data_buff(int serv_fd, char *mbuff, int len)
{
    int ret, i, last;

  /*  prepare to send message  */
    if (echo_fp)  fprintf(echo_fp,">> DATA\n");
    write(serv_fd,"DATA\n",strlen("DATA\n"));
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 300 || ret > 399 )
                return 1;   /* some form of error message */

  /*  copy buffer  */
    if (echo_fp)  fprintf(echo_fp,"<< sending %d bytes of data >>\n",len);
    for(i=0,last=0;i<len;i++) {
        if ( mbuff[i] != '\n'  && i<len-1 ) continue;
      /*  check first character of each line for full stops  */
        if ( mbuff[last] == '.' )
                write(serv_fd,".",1);  /*  double up initial full stop  */
        write(serv_fd,&mbuff[last],i-last+1);
        last=i+1;
    }
    if ( mbuff[len-1] != '\n' ) write(serv_fd,"\n",1); 
  /*  message now complete, send terminating full stop  */
    if (echo_fp)  fprintf(echo_fp,">> .\n");
    write(serv_fd,".\n",2);
    ret = get_response_fd( serv_fd, 2, NULL, 0 );
    if ( ret < 200 || ret > 299 )
                return 1;   /* some form of error message */
    return 0;
}

