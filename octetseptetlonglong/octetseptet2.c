/**
 * linux :
 * $CC -Wall -ggdb3 octetseptet2.c -D_LE_ -lm -o octetseptet2
 *
 * aix: 
 * $CC -Wall -ggdb3 octetseptet2.c -D_BE_ -lm -o octetseptet2
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
/**
 * Name		: gettlvlength 
 * Description	: get length defined by this stream
 *		  the stream format is big endian, with first byte indicates
 *		  whether there are any bytes follows this current byte.
 * Input	
 *	in	: stream to interpret
 *	maxlength : maximum length to interpret
 *		
 **/

long long int gettlvlength(unsigned char *in, int maxlength)
{
	unsigned long long int res = 0;
	int i = 0;
	int maxbytes = 0;	
	//unsigned char *y;
	
	for (i = 0; i < maxlength; i++)
	{
		if (!(in[i] & 0x80))
			break;
	} 
	if (i==maxlength && ((in[i-1]&0x80))) 
		return 0;
	maxbytes = i;
	//y = (unsigned char *) calloc (1, maxbytes+1);
	//if (y == NULL)
	//	return 0;
	for (i = 0; i<=maxbytes; i++)
	{
#ifdef _LE_
		//y[i] = in[maxbytes -i] & 0x7F; 
		res+= (in[maxbytes-i] & 0x7F) * pow (2, (8*i - i));
#else
		//y[i] = in[i] & 0x7F; 
		res+= (in[i] & 0x7F) * pow (2, (8*(maxbytes-i) - (maxbytes-i)));
#endif 
	}
	return res;
}

int64_t int_gettlvlength(unsigned char *in, int maxlength)
{
	int64_t res = 0;
	int i = 0; 
	res |= (in[i]&0x7F) ;
	while ( (in[i] & 0x80))
	{
		res <<=7;
		i++; 
		res |= (in[i]&0x7F);
	}
	return res;
}
/**
 * Name		: settlvlength
 * Description	: set tlv length , base 128
 * Input
 *		in : values to set in tlv
 * 		maxout : max output size
 * Output	out : array to put the tlv length
 * Returns	: the length in bytes of written tlv length
 **/
int settlvlength (long long int in, unsigned char *out, int maxout)
{ 
	int i = 0;
	int maxbytes = 0;
	int halfbyte = 0; 

	unsigned char kern = 0x00;
	unsigned char ctemp = 0x00;
	long long int temp = pow (2, 8*i-i);
	long long int y = in/temp;
	do
	{
		out[i] = y & 0x7F;
		out[i]|= kern;
		kern|=0x80;
		i++;
		temp = pow (2, 8*i-i);
		y = in/temp; 
	}while ((y>0) && (i <maxout));

	maxbytes = i;
	halfbyte = i/2;
	for (i=0; i<halfbyte; i++)
	{
		ctemp = out[i];
		out[i] = out[maxbytes-1-i];
		out[maxbytes-1-i] = ctemp;
	} 
	return maxbytes;
}
int int_settlvlength(int64_t in, unsigned char *out, int maxout)
{
	int i = 0;
	int maxbytes = 0;
	int halfbyte = (maxbytes>>1); 
	unsigned char ctemp = 0x00;
	out[i] = (in & 0x7F);
	in>>=7;	
	i++;
	while ((in>0) && (i<=maxout))
	{
		out[i] = (in & 0x7F);
		out[i]|=0x80;	
		in >>= 7; 
		i++;
	}
	maxbytes = i; 
	for (i=0; i<halfbyte; i++)
	{
		ctemp = out[i];
		out[i] = out[maxbytes-1-i];
		out[maxbytes-1-i] = ctemp;
	} 

	return maxbytes;
}
int printTlvLength (long long int in, unsigned char *buffer, int maxbuffer)
{
	int max = 0;
	int i = 0; 

	max = settlvlength (in, buffer, maxbuffer);
	for (i = 0; i<max; i++)
	{
		fprintf (stdout, "[%02X]", buffer[i]);
	}
	fprintf (stdout, "\n");	
	return max;
}

int int_printTlvLength (int64_t in, unsigned char *buffer, int maxbuffer)
{
	int max = 0;
	int i = 0; 

	max = int_settlvlength (in, buffer, maxbuffer);
	for (i = 0; i<max; i++)
	{
		fprintf (stdout, "[%02X]", buffer[i]);
	}
	fprintf (stdout, "\n");	
	return max;
}

int cont = 1;
int j = 0;


void BtoH(char* pDstHex, unsigned char* pSrc, int nSrcLen)
{
   int i;
   for (i = 0; i < nSrcLen; ++i)
   {
      *pDstHex = ((pSrc[i] >> 4) & 0x0F);
      if (*pDstHex < 10)
         *pDstHex += '0';
      else
         *pDstHex += 'A' - 10;
      ++pDstHex;
      *pDstHex = (pSrc[i] & 0x0F);
      if (*pDstHex < 10)
         *pDstHex += '0';
      else
         *pDstHex += 'A' - 10;
      ++pDstHex;
   }
}

void HtoB(unsigned char * pDst, char* pSrcHex, int nSrcLenHex)
{
   char c;
   int nNibble1;
   int nNibble2;
   int i;
   for (i = 0; i < nSrcLenHex; i += 2)
   {
      c = pSrcHex[i];
      if (c >= '0' && c <= '9')
         nNibble1 = c - '0';
      else if (c >= 'A' && c <= 'F')
         nNibble1 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble1 = c - 'a' + 10;
      else
         nNibble1 = 0;
      c = pSrcHex[i+1];
      if (c >= '0' && c <= '9')
         nNibble2 = c - '0';
      else if (c >= 'A' && c <= 'F')
         nNibble2 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble2 = c - 'a' + 10;
      else
         nNibble2 = 0;
      *pDst++ = (nNibble1 << 4) + nNibble2;
   }

} 
int usage (char *appName, FILE *fp)
{
	fprintf (fp, "usage : %s [-h][-t] stream\n", appName);
	fprintf (fp, "-h tlv length to hexa\n");
	fprintf (fp, "-t length to tlv\n");
	return 0;	
}
int main (int argc, char **argv)
{ 
	unsigned char result[12];
	int64_t res = 0;
	int max = 0;
	int i = 0;
	if (argc <3)
	{
		usage(argv[0], stderr);
		exit (EXIT_FAILURE);
	}
	if (!strcmp (argv[1], "-h"))
	{ 
		memset (result, 0, 12);
		HtoB (result, argv[2], strlen(argv[2]));
		res = int_gettlvlength (result,strlen(argv[2])/2);
		fprintf (stdout, "length = %lld\n", res);
	}
	else if (!strcmp (argv[1], "-t"))
	{
		res = atoi (argv[2]);
		max = int_settlvlength (res,result, 12); 
		for (i = 0; i<max; i++)
		{
			fprintf (stdout, "%02X",result[i]);
		}
		fprintf (stdout, "\n");
	}
	exit (EXIT_SUCCESS);
}
