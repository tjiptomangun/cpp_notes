/****************************************/
/*                                      */
/*      sock.h                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      package up socket garbage       */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#ifndef SOCK_HEAD
#define SOCK_HEAD

#ifdef ANSI

int  parse_network_args( int *argc, char **argv, char **hostname,
                         int *portno, char **errmess );
int  tcp_passive_open(int portno);
int  tcp_active_open(char *hostname,int portno);
int  tcp_accept(int sock);

#else

int  parse_network_args();
int  tcp_passive_open();
int  tcp_active_open();
int  tcp_accept();

#endif


#endif  /* SOCK_HEAD  */
