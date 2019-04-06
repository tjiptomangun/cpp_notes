/****************************************/
/*                                      */
/*      sock.h                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      helper functions for common     */
/*      Internet application protocols  */
/*                                      */
/*      21st January 1996               */
/*                                      */
/****************************************/

#ifndef PROTOCOL_HEAD
#define PROTOCOL_HEAD

#ifdef ANSI

int get_response_fp( FILE *server_fp, FILE *echo_fp, char *buff, int len );
int get_response_fd( int server_fd, int echo_fd, char *buff, int len );

#else

int get_response_fp();
int get_response_fd();

#endif


#endif  /* PROTOCOL_HEAD*/
