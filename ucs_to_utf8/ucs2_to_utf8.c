/*
gcc -Wall -ggdb3 ucs2_to_utf8.c -o ucs2_to_utf8
http://www.lemoda.net/c/ucs2-to-utf8/
*/

/* Input: a Unicode code point, "ucs2". 

   Output: UTF-8 characters in buffer "utf8". 

   Return value: the number of bytes written into "utf8", or -1 if
   there was an error.

   This adds a zero byte to the end of the string. It assumes that the
   buffer "utf8" has at least four bytes of space to write to. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define UNICODE_BAD_INPUT -1
#define UNICODE_SURROGATE_PAIR -2

int ucs2_to_utf8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
	if (ucs2 >= 0xD800 && ucs2 <= 0xDFFF) {
	    /* Ill-formed. */
	    return UNICODE_SURROGATE_PAIR;
	}
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    if (ucs2 >= 0x10000 && ucs2 < 0x10FFFF) {
	/* http://tidy.sourceforge.net/cgi-bin/lxr/source/src/utf8.c#L380 */
	utf8[0] = 0xF0 | (ucs2 >> 18);
	utf8[1] = 0x80 | ((ucs2 >> 12) & 0x3F);
	utf8[2] = 0x80 | ((ucs2 >> 6) & 0x3F);
	utf8[3] = 0x80 | ((ucs2 & 0x3F));
        utf8[4] = '\0';
        return 4;
    }
    return UNICODE_BAD_INPUT;
}

int hextoint (char *in)
{
	int nLen = strlen (in);
	char *buff = in;
	int nAccum = 0, nCurr = 0, x=0;
	
	for(x = 0; x < nLen; x++)
	{
	       nCurr = 0;
               if(! ((buff[x]  >= '0' && buff[x] <= '9') ||
                      (buff[x] >= 'A' && buff[x] <= 'F') ||
                      (buff[x] >= 'a' && buff[x] <= 'f') ))
                {
                        fprintf(stderr,"non hex\n");
                        exit(0);
                }
		if(buff[x] >= '0' && buff[x] <= '9')
			nCurr = buff[x] - '0';
		
		if(buff[x] >= 'A' && buff[x] <= 'F')
			nCurr= (buff[x] - 'A') + 10;

		if(buff[x] >= 'a' && buff[x] <= 'f')
			nCurr= (buff[x] - 'a') + 10;

		nAccum *= 16;
		nAccum += nCurr; 
	}
	return nAccum;
	
}
int printhex(unsigned char *buff)
{
	int len = strlen ((char *)buff);
	int i = 0;
	for ( i = 0 ; i<len; i++)
	{
		fprintf (stdout, "%02X", buff[i]);
	}
	return 0;
}

int main (int argc, char **argv)
{
	int x = -1;
	unsigned char buff [8];
	memset (buff, 0, 8);
	if (argc < 2)
	{
		fprintf (stderr, "usage :%s ucs2_in_hex\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	x = hextoint (argv[1]);	
	ucs2_to_utf8 (x, buff);
	printhex(buff);	
	fprintf (stdout, "\n");
	exit (EXIT_SUCCESS);	
}
