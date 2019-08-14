/**
 * gcc -Wall -ggdb3 octettest.c -o octettest
 */

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stddef.h>
#include "cgi.h"
#include <assert.h>
#include <stdbool.h>

/**
 * NAME         : str_to_short
 * DESCRIPTION  : convert array of 4 char to short
 * INPUT
 *          in  : pointer to input
 *         out  : pointer to output
 * RETURNS
 *     length of bytes copied to output
 */
int str_to_short(char *in, char *out) {
  char *inptr = in;
  char *outptr = out;
  int ret = 0;
  if (inptr[0] >= 'a' && inptr[0] <= 'f'){
    ret |= (inptr[0] - 'a') + 10;
    inptr ++;
  }
  else if (inptr[0] >= 'A' && inptr[0] <= 'F'){
    ret |= (inptr[0] - 'A') + 10;
    inptr ++;
  }
  else {
    ret |= (inptr[0] - '0');
    inptr ++;
  }
  ret <<= 4;
  if (inptr[0] >= 'a' && inptr[0] <= 'f'){
    ret |= (inptr[0] - 'a') + 10;
    inptr ++;
  }
  else if (inptr[0] >= 'A' && inptr[0] <= 'F'){
    ret |= (inptr[0] - 'A') + 10;
    inptr ++;
  }
  else {
    ret |= (inptr[0] - '0');
    inptr ++;
  }
  ret <<= 4;
  if (inptr[0] >= 'a' && inptr[0] <= 'f'){
    ret |= (inptr[0] - 'a') + 10;
    inptr ++;
  }
  else if (inptr[0] >= 'A' && inptr[0] <= 'F'){
    ret |= (inptr[0] - 'A') + 10;
    inptr ++;
  }
  else {
    ret |= (inptr[0] - '0');
    inptr ++;
  }
  ret <<= 4;
  if (inptr[0] >= 'a' && inptr[0] <= 'f'){
    ret |= (inptr[0] - 'a') + 10;
    inptr ++;
  }
  else if (inptr[0] >= 'A' && inptr[0] <= 'F'){
    ret |= (inptr[0] - 'A') + 10;
    inptr ++;
  }
  else {
    ret |= (inptr[0] - '0');
    inptr ++;
  }

  if (ret >= 10000){
    outptr[0] = (ret/10000) + '0';
    outptr ++;
    ret = ret %10000;
  }
  if (ret >= 1000){
    outptr[0] = (ret/1000) + '0';
    outptr ++;
    ret = ret %1000;
  }
  if (ret >= 100){
    outptr[0] = (ret/100) + '0';
    outptr ++;
    ret = ret %100;
  }
  if (ret >= 10){
    outptr[0] = (ret/10) + '0';
    outptr ++;
    ret = ret %10;
  }
  outptr[0] = ret + '0';
  outptr ++;
  return outptr - out; 
}

/**
 * NAME         : cgi_lai
 * DESCRIPTION  : parse cgi request bytes to ldap response
 * INPUT
 *      version : method version 1 or 2
 *      in      : message to parse
 *      out     : the result
 *      is_cgi  : true if it is cgi, otherwise it is lai
 * RETURNS
 *      Success : number of bytes copied to out buffer
 *      Error   : CGI_E_INVALID_VERSION , version not supported
 *
 */
int cgi_lai(unsigned int version, char *in, char *out, bool is_cgi)  {
  char *inptr = in;
  char *outptr = out;
  int ret = 0;
  if (version !=1 && version != 2){
    return CGI_E_INVALID_VERSION;
  }
  if (version == 2){ 
    out[1] = in[0];
    out[0] = in[1];
    outptr += 2;
    inptr += 2;
    if (in[3] != 'f' && in[3] != 'F'){
      outptr[0] = in[3];
      outptr ++;
    }
    if (in[2] != 'f' && in[2] != 'F'){
      outptr[0] = in[2];
      outptr ++;
    }//mcc
    inptr+=2; 
    outptr[0] = '-';
    outptr ++;

    outptr[0] = inptr[1];
    outptr ++;
    if (inptr[0] != 'f' && in[3] != 'F'){
      outptr[0] = inptr[0];
      outptr ++;
    }
    inptr +=2;
    outptr[0] = '-';
    outptr ++;
  }
  else {
    inptr +=6;
  }
  ret = str_to_short(inptr, outptr);
  inptr +=4;
  outptr += ret;
  outptr[0] = '-';
  outptr ++;
  if (is_cgi) {
    ret = str_to_short(inptr, outptr);
    outptr += ret;
  }
  else {
      outptr[0] = '0';
      outptr ++;
  }
  return outptr - out;
}
#ifdef CGI_UNIT_TEST
int main (int argc, char **argv) {
// char test[] = {"25f0101ad2868d"};
// char test[] =   {"35f1121bd28681"};
 char test[] = {"25F0231B124A8D"};
// char test[] = {"25f2231f121aff"};
// char test[] = {"25F03204C622FA"};
//  char test[] = {"25f0931f1f24f0"};
  char out[18]; 
  memset (out, 0, 18);
  int ret = cgi_lai(2, test, out, 14);
  printf("%s\n", out);
  assert(ret > 0);

  memset (out, 0, 18);
  ret = cgi_lai(1, test, out, 14);
  printf("%s\n", out);
  assert(ret > 0);
  memset (out, 0, 18);
  ret = cgi_lai(0, &test[6], out, 14);
  assert(ret < 0);
}
#endif

#ifdef CGI_PERF_TEST
#include <time.h>
#include <unistd.h>
#define BILLION 1000000000.0

int main (int argc, char **argv) {
 // char test[] = {"25f0101ad2868d"};
  char test[] =   {"35f1101ad2868d"};
  char out[18]; 
  struct timespec start, end;
  memset (out, 0, 18); 
  clock_gettime(CLOCK_REALTIME, &start); 
  cgi_lai(2, test, out, 14);
  clock_gettime(CLOCK_REALTIME, &end); 
  double time_spent = (end.tv_nsec - start.tv_nsec); 
  printf ("Time elapsed is %f secs\n", time_spent/BILLION); 
}
#endif


