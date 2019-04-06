/*******************************************/
/*                                         */
/*      mail-helper.h                      */
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

FILE *do_mail_set_echo_fp(FILE *echo_fp);
int do_mail_init(int serv_fd);
int do_mail_from(int serv_fd, char *from);
int do_mail_to(int serv_fd, char *to);
int do_mail_data_fp(int serv_fd, FILE *user_fp);
int do_mail_data_buff(int serv_fd, char *buff, int len);
int do_mail_quit(int serv_fd);


