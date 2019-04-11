#ifndef CGI__H
#define CGI__H

#define CGI_E_INVALID_FORMAT -1
#define CGI_E_INSUFFICIENT_SIZE -2
#define CGI_E_INVALID_VERSION -3
int cgi(unsigned int version, char *in, char *out);
#endif
