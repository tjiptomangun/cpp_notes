#ifndef CGI__H
#define CGI__H
#include <stdbool.h>

#define CGI_E_INVALID_FORMAT -1
#define CGI_E_INSUFFICIENT_SIZE -2
#define CGI_E_INVALID_VERSION -3
int cgi_lai(unsigned int version, char *in, char *out, bool is_cgi);
int str_to_short(char *in, char *out);
#endif
