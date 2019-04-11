/**
 * gcc -Wall -ggdb3 octettest.c -o octettest
 */

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "octettest.h"

/**
 * NAME		: bcd_to_hex
 * DESCRIPTION	: Convert bcd bytes to common hex bytes.
 * INPUT 
 * 	inchar 	: bytes to convert, should not overlap outchar
 * 	outchar	: converted bytes, should not overlap inchar
 * 	inbytes : number of bytes to convert
 * 	outbytes: output buffer size in bytes
 *
 * RETURNS
 * 	Success	: number of bytes copied
 * 	-1	: invalid format
 * 	-2 	: insufficient outbytes size
 *
 * 
 */
size_t  bcd_to_hex(unsigned char *inchar, char *outchar, size_t inbytes, size_t outbytes) {
	int i = 0, j = 0;
	int remain = outbytes;
	for (i = 0, j = 0; i < inbytes; i++){
		if ((inchar[i] & 0x0F) < 0x09){
			if (remain > 0){
				outchar[j++] = inchar[i] & 0x0F;
				remain --;
			}
			else{
				return -2;
			}
		}
		else 
			return ERROR_ONE; 

		
		if ((inchar[i] & 0xF0) ==  0xF0)
			break; 
		else if ((inchar[i] >>4 & 0x0F) < 0x09){
			if (remain > 0){
				outchar[j++] = inchar[i]>>4 & 0x0F;
				remain --;
			}
			else {
				return -2;
			}
		}
		else{
			return -1;
		}
	}
	return j;
	
} 

/* convert hex bytes to hex string */
void bytes_to_string(char *inchar, size_t charlength) {
	int i = 0;
	for (i = 0; i < charlength ; i++){ 
		if (inchar[i]  < 9)
			inchar[i] += '0';
		else if(inchar[i]  <= 0x0F)
			inchar[i] = (inchar[i] - 10) + 'f';
	}
}

int cgi(unsigned int version, unsigned char *in, char *out, size_t inlength, size_t outlength)  {
	int len = 0;
	int converted = 0;
	char *outptr = out;
	unsigned char *inptr = in;
	unsigned int lac = 0;
	unsigned int ci = 0;
	size_t remain = outlength;

	if (version < 1 && version > 2)
		return -1; 
	else if (inlength < 7){
		return -1 ;
	}
	else if (outlength < 10){
		return -1;
	}
	if (version == 2) {
		/*MCC*/
		len = bcd_to_hex(inptr, outptr, 2, remain);
		bytes_to_string(outptr, len); 
		converted +=len;
		outptr += len;
		inptr += 2;
		remain = remain - len;
		outptr[0] = '-';
		outptr ++;
		remain --;
		/*MNC*/
		len = bcd_to_hex(inptr, outptr, 1, remain);
		bytes_to_string(outptr, len); 
		converted += len;
		outptr += len;
		remain -= len;
		inptr += 1;
		outptr[0] = '-';
		outptr ++;
		remain --;
	} 
	lac = inptr[0] << 8 | inptr[1];
	inptr += 2;
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

	snprintf(outptr, remain, "%d", ci); 
	len = strlen(outptr);

	remain -= len; 
	
	return outlength - remain;
}

int main (int argc, char **argv) {
	unsigned char test[] = {0x25, 0xf5, 0x23, 0x1a, 0xd2, 0x86, 0x8d, 0};
	char out[18]; 
	
	memset(out, 0, 18);
	cgi(2, test, out, 7, 20);
	fprintf(stdout, "%s\n", out);
	memset(out, 0, 18);
	cgi(1, &test[3], out, 7, 20);
	fprintf(stdout, "%s\n", out);
}
