/****************************************/
/*                                      */
/*      protocol.c                      */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      helper functions for common     */
/*      Internet application protocols  */
/*                                      */
/*      21st January 1996               */
/*                                      */
/****************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "protocol.h"

#define BUFF_LEN 200

int get_response_fp( FILE *server_fp, FILE *echo_fp,
                     char *res_buff, int blen )
{
    char my_buff[BUFF_LEN+1];
    char *bp = res_buff;
    int  rem_len = blen-1;
    my_buff[BUFF_LEN] = '\0';
    for(;;) {
        if ( fgets(my_buff,BUFF_LEN,server_fp) == NULL ) {
            if ( bp ) *bp = '\0';
            return -1;    /*  connection lost  */
            }
        if ( echo_fp ) fputs(my_buff,echo_fp);
        if ( bp ) {
            int n = strlen(my_buff);
            if ( n >= rem_len ) n = rem_len;
            strncpy(bp,my_buff,n);
            rem_len -= n;
            bp += n;
            if ( rem_len <= 0 ) {
                *bp = '\0';
                bp == NULL;
                }
            }
        if ( isdigit(my_buff[0]) && my_buff[3] == ' ' ) {
                /*  last line of response  */
            my_buff[3] = '\0';
            if ( bp ) *bp = '\0';
            return atoi(my_buff);
            }
        }
}


int get_response_fd( int server_fd, int echo_fd,
                     char *res_buff, int blen )
{
    char c;
    char resp[4];
    char my_buff[BUFF_LEN];
    char *bp = res_buff;
    int  rem_len = blen-1;
    int n, i;
    int pos = 0;

    while ( ( n = read( server_fd, my_buff, BUFF_LEN ) ) > 0 ) {
        if ( echo_fd >= 0 ) write(echo_fd,my_buff,n);
        if ( bp ) {
            if ( n >= rem_len ) n = rem_len;
            memcpy(bp,my_buff,n);
            rem_len -= n;
            bp += n;
            if ( rem_len <= 0 ) {
                *bp = '\0';
                bp == NULL;
                }
            }
        for ( i=0; i<n; i++ ) {
            if ( my_buff[i] != '\n' ) {
                if ( pos < 4 ) resp[pos++] = my_buff[i];
                continue;
                }
            if ( pos == 4 && isdigit(resp[0]) && resp[3] == ' ' ) {
                    /*  last line of response  */
                resp[3] = '\0';
                if ( bp ) *bp = '\0';
                return atoi(resp);
                }
            pos = 0;
            }
        }
    if ( bp ) *bp = '\0';
    return -1;    /*  connection lost  */
}




