//gcc h2b2.c -Wall -ggdb3 -o h2b2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void HtoB2 (unsigned char * pDst, char *pSrcHex, int srcLen)
{
	int i = 0;
	unsigned char temp;
	char *currSrc = pSrcHex;
	unsigned char *currDst = pDst;
	do 
	{
		*currDst = 0;
		for (i = 0; i < 2; i++)
		{
			*currDst<<=4;
			temp = *currSrc;
			temp &= 0x4F;
			*currDst += (temp >> 6)*10 + (temp & 0xF) - (temp >> 6); 
			currSrc ++;
			srcLen --;
		}
		currDst ++; 
	} while (srcLen);
}

int main (int argc, char **argv)
{
	char *src ;
	unsigned char target [600]; 
	int len = 0;
	int i = 0;
	memset (target, 0, 600);

	if (argc < 2)
	{
		fprintf (stderr,"usage : %s argument\n", argv[0]);
		exit (EXIT_FAILURE);

	}
	len = strlen (argv[1]);
	src = argv[1];

	if (len&0x01)
	{
		fprintf (stderr, "error length, should be even\n");
		exit (EXIT_FAILURE);
	}
	
	HtoB2 (target, argv[1], len);
	for (i = 0; i< len/2 ; i++)
	{

		fprintf (stdout, "%X", target[i]);
	}
	fprintf (stdout, "\n");

	exit (EXIT_SUCCESS);
}
