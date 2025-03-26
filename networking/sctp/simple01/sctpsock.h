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

#ifndef SCTPSOCK_HEAD
#define SCTPSOCK_HEAD

#ifdef ANSI

int  parse_network_args( int *argc, char **argv, char **hostname,
                         int *portno, char **errmess );
int  sctp_passive_open(int portno);
int  sctp_active_open(char *hostname,int portno);
int  sctp_accept(int sock);

#else

int  parse_network_args(int *argc, char **argv, char **hostname, int *portno, char **errmess);
int  sctp_passive_open(int portno);
int  sctp_active_open(char * hostname, int portno);
int  sctp_accept(int sock);

#endif


#endif  /* SCTPSOCK_HEAD  */
