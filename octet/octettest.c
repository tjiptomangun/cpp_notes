/**
 * gcc -Wall -ggdb3 octettest.c -o octettest
 */

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

/**
 * convert bcd bytes to common hex bytes
 * returns length of copied bytes
 */
size_t  bcd_to_hex(char *inchar, char *outchar, size_t inbytes, size_t outbytes) {
	int i = 0, j = 0;
	for (i = 0, j = 0; i < inbytes; i++){
		if ((inchar[i] & 0xF0) != 0xF0)
			outchar[j++] = inchar[i] & 0x0F;
		else
			break;


		if (j + 1 > outbytes)
			break;
		else if ((inchar[i] & 0xF0) != 0xF0)
			outchar[j++] = inchar[i]>>4 & 0x0F;
		else
			break;
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

int cgi(unsigned int version, char *in, char *out, size_t inlength, size_t outlength)  {
	int len = 0;
	int converted = 0;
	char *outptr = out;
	char *inptr = in;
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
	lac = (unsigned char)inptr[0] << 8 | (unsigned char)inptr[1];
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
	ci = (unsigned char)inptr[0] << 8 | (unsigned char)inptr[1];

	snprintf(outptr, remain, "%d", ci); 
	len = strlen(outptr);

	remain -= len; 
	
	return outlength - remain;
}

int main (int argc, char **argv) {
	char test[] = {0x25, 0xf5, 0x23, 0x1a, 0xd2, 0x86, 0x8d, 0};
	char out[14];
	int len = 0;
	
	memset(out, 0, 14);
	cgi(2, test, out, 7, 20);
	fprintf(stdout, "%s\n", out);
	memset(out, 0, 14);
	cgi(1, &test[3], out, 7, 20);
	fprintf(stdout, "%s\n", out);
}
