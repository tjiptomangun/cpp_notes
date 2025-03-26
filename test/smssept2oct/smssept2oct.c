#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define BYTE			unsigned char

int Dec2CharHex (BYTE nBil, char * sBil)
{
	sprintf(sBil, "%02x", nBil);

	if (sBil[0] > 0x60 && sBil[0] < 0x67)
	{
		sBil[0] = sBil[0] & 0x4F;
	}

	if (sBil[1] > 0x60 && sBil[1] < 0x67)
	{
		sBil[1] = sBil[1] & 0x4F;
	}

	return (1);

}

int main()
{
       char sText[100];
       char sBuff[100];
       int 	nAnd, nPow;
       int 	nIndex = 0;
       int 	nIndx = 0;
       int 	nTextLen ;
       char 	szTemp[3];
       BYTE 	nHasil = 0;
       char 	*pTemp;
       memset(sText,0,100);
       memset(sBuff,0,100);

       sprintf(sText,"%s","Hello World Of Top");
       nTextLen = strlen (sText);
       if(nTextLen)
       {
             pTemp = (char *) malloc (nTextLen+1);
       }
       else
       {
             return 0;
       }

       memset (pTemp, 0, nTextLen + 1);
       memcpy (pTemp, sText, nTextLen);

       while (nIndx < nTextLen )
       {
              if (nIndex % 7 == 0 && nIndex != 0)
              {
                 nIndx++;
              }
              nPow = (int) (pow (2.0, (nIndex % 7) + 1));
              nAnd = (pTemp[nIndx+1]) & (nPow - 1);
              nAnd <<= 7 - (nIndex % 7);
              pTemp[nIndx+1] >>= (nIndex % 7) + 1;
              nHasil = pTemp[nIndx] + nAnd;
              Dec2CharHex (nHasil, szTemp);
              strcat (sBuff, szTemp);
              printf("%d. %s\n",nIndx,sBuff);
              nIndex++;
              nIndx++;
       }
       strcat	(sBuff, "\0");
       free(pTemp);
       //printf("%s\n",sBuff);
       return (nIndex);
}
