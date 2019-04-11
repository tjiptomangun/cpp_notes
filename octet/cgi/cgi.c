/**
 * gcc -Wall -ggdb3 octettest.c -o octettest
 */

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stddef.h>
#include "cgi.h"
#include <assert.h>

/**
 * NAME   : bcd_to_hex
 * DESCRIPTION  : Convert bcd bytes to common hex bytes.
 * INPUT 
 *  inchar  : bytes to convert, should not overlap outchar
 *  outchar : converted bytes, should not overlap inchar
 *  inbytes : number of bytes to convert
 *  outbytes: output buffer size in bytes
 *
 * RETURNS
 *  Success : number of bytes copied
 *  Error   : CGI_E_INVALID_FORMAT  , invalid format
 *            CGI_E_INSUFFICIENT_SIZE  , insufficient outbytes size
 */
int bcd_to_hex(unsigned char *inchar, char *outchar, size_t inbytes, size_t outbytes) {
  int i = 0, j = 0;
  int remain = outbytes;
  for (i = 0, j = 0; i < inbytes; i++){
    if ((inchar[i] & 0x0F) < 0x09){
      if (remain > 0){
        outchar[j++] = inchar[i] & 0x0F;
        remain --;
      }
      else{
        return CGI_E_INSUFFICIENT_SIZE;
      }
    }
    else 
      return CGI_E_INVALID_FORMAT; 
    
    if ((inchar[i] & 0xF0) ==  0xF0)
      break; 
    else if ((inchar[i] >>4 & 0x0F) < 0x09){
      if (remain > 0){
        outchar[j++] = inchar[i]>>4 & 0x0F;
        remain --;
      }
      else {
        return CGI_E_INSUFFICIENT_SIZE;
      }
    }
    else{
      return CGI_E_INVALID_FORMAT;
    }
  }
  return j;
  
} 

/**
 * NAME         : bytes_to_string
 * DESCRIPTION  : convert array of bytes to ascii string
 * INPUT
 *      inchar  : characters to convert
 *      charlength : number of char to covert
 * RETURNS
 *      NONE
 */
void bytes_to_string(char *inchar, size_t charlength) {
  int i = 0;
  for (i = 0; i < charlength ; i++){ 
    if (inchar[i]  < 9)
      inchar[i] += '0';
    else if(inchar[i]  <= 0x0F)
      inchar[i] = (inchar[i] - 10) + 'f';
  }
}
/**
 * NAME         : cgi
 * DESCRIPTION  : parse cgi request bytes to ldap response
 * INPUT
 *      version : method version 1 or 2
 *      in      : message to parse
 *      out     : the result
 *      inlength: number of bytes to convert
 *      outlength: result buffer size
 * RETURNS
 *      Success : number of bytes copied to out buffer
 *      Error   : CGI_E_INVALID_FORMAT  , invalid format
 *                CGI_E_INSUFFICIENT_SIZE  , insufficient outbytes size
 *                CGI_E_INVALID_VERSION , version not supported
 *
 */
int cgi(unsigned int version, unsigned char *in, char *out, size_t inlength, size_t outlength)  {
  int len = 0;
  int converted = 0;
  char *outptr = out;
  unsigned char *inptr = in;
  unsigned int lac = 0;
  unsigned int ci = 0;
  int remain = outlength;

  if (version < 1 && version > 2)
    return CGI_E_INVALID_VERSION; 
  else if (inlength < 7){
    return CGI_E_INVALID_FORMAT ;
  }

  if (version == 2) {
    /*MCC*/
    len = bcd_to_hex(inptr, outptr, 2, remain);
    if (len < 0)
            return len;
    bytes_to_string(outptr, len); 
    converted +=len;
    outptr += len;
    inptr += 2;
    remain = remain - len;
    if (remain <= 5)
            return CGI_E_INSUFFICIENT_SIZE;
    outptr[0] = '-';
    outptr ++;
    remain --;
    /*MNC*/
    len = bcd_to_hex(inptr, outptr, 1, remain);
    if (len < 0)
            return len;
    bytes_to_string(outptr, len); 
    converted += len;
    outptr += len;
    remain -= len;
    inptr += 1;
    if (remain <= 3)
            return CGI_E_INSUFFICIENT_SIZE;
    outptr[0] = '-';
    outptr ++;
    remain --;
  } 
  lac = inptr[0] << 8 | inptr[1];
  inptr += 2;
  if ((lac>= 10000 && remain < 6) || (lac >= 1000 && remain < 5) || (lac >= 100 && remain < 4) || (lac >= 10 && remain < 3)){
            return CGI_E_INSUFFICIENT_SIZE;
  }
  /*LAC*/
  snprintf(outptr, remain, "%d", lac);
  len = strlen(outptr);
  outptr += len;
  remain -= len; 
  outptr[0] = '-';
  outptr ++;
  remain --;

  /*CI*/
  ci = inptr[0] << 8 | inptr[1];
  if ((ci>= 10000 && remain < 5) || (ci >= 1000 && remain < 4) || 
        (ci >= 100 && remain < 3) || (lac >= 10 && remain < 2)){
            return CGI_E_INSUFFICIENT_SIZE;
  }

  snprintf(outptr, remain, "%d", ci); 
  len = strlen(outptr);

  remain -= len; 
  
  return outlength - remain;
}
#ifdef TEST
int main (int argc, char **argv) {
  unsigned char test[] = {0x25, 0xf5, 0x23, 0x1a, 0xd2, 0x86, 0x8d, 0};
  char out[18]; 
  int ret;

  
  memset(out, 0, 18);
  ret = cgi(2, test, out, 7, 18);
  assert(ret > 0);
  fprintf(stdout, "%s\n", out);
  memset(out, 0, 18);
  ret = cgi(1, &test[3], out, 7, 18);
  assert(ret > 0); 
  fprintf(stdout, "%s\n", out);

  char out2[14]; 
  ret = cgi(2, test, out2, 7, 14);
  assert(ret < 0); 

}
#endif
