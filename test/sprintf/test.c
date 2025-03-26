#include <stdio.h>
#define BOOLEAN int
#define USINT16 unsigned short
#define ISO_AMOUNT_SIZE				12
#define TRUE 1
#define FALSE 0
int IsoMapOutRAmountDouble (char *pDest, double dSrc, int nDestLen, BOOLEAN bSign, USINT16 nDecimalRule);

int main(int argc, char ** argv)
{
	//double arg = 232000000000000;
	double arg = 40323050420.00;
        char sBuf[256];
        int nLen;
        char pStream[256];

        //sprintf(sBuf,"%0*.2f",1,arg);

        //printf("%s\n",sBuf);
        memset(pStream,0,256);

    nLen = IsoMapOutRAmountDouble (&pStream[0], arg,
									ISO_AMOUNT_SIZE, FALSE, 2);
       printf("[%06d]  %s\n",nLen,pStream);
}



int IsoMapOutRAmount (char *pDest, char *pSrc, int nDestLen, int nSrcLen,
                      BOOLEAN bSign, USINT16 nDecimalRule)
{
    int			nLen;
    int			nDestLen2;

    if (bSign)
    {
        switch (pSrc[0])
        {
            case '+':
                pDest[0] = 'C';
                ++pSrc;
                break;

            case '-':
                pDest[0] = 'D';
                ++pSrc;
                break;

            default:
                pDest[0] = 'C';
                break;

        }
        ++pDest;
        --nSrcLen;
    }
    else
    {
        switch (pSrc[0])
        {
            case '+':
            case '-':
                /*	Drop the sign  */
                ++pSrc;
                break;
        }
    }

    memset (pDest, '0', nDestLen);
    nLen = strlen (pSrc);
    nDestLen2 = nDestLen;
    switch (nDecimalRule)
    {
        case 0:	/*	No Change  */
            break;

        case 1: /*	Truncate  */
            /*	TBE: 970912 Different host may want to receive the amount
                differently. Some with decimal, and some without decimal.
            */
            if (nLen > 2)
            {
                nLen -= (pSrc[nLen - 3] == '.' ? 3 : 2);
            }
            break;

        case 2: /*	Append  */
            nDestLen2 -= 2;
            break;
    }
    if (nLen > nSrcLen)
    {
        nLen = nSrcLen;
    }
    if (nLen > nDestLen2)
    {
        nLen = nDestLen2;
    }
    memcpy (&pDest[nDestLen2-nLen], pSrc, nLen);
    return (bSign ? nDestLen+1 : nDestLen);

} /* End of IsoMapOutRAmount */


int IsoMapOutRAmountDouble (char *pDest, double dSrc, int nDestLen, BOOLEAN bSign, USINT16 nDecimalRule)
{
    char			sBuf[32];

    switch (nDecimalRule)
    {
        case 0:	/*	No change. Not applicable? Treat as Truncate?  */
        case 1:	/*	Truncate  */
            sprintf (sBuf, "%0*.0f", nDestLen, dSrc);
            break;

        case 2: /*	Append  */
            sprintf (sBuf, "%0*.2f", nDestLen+1, dSrc);
            memmove (&sBuf[nDestLen-2], &sBuf[nDestLen-1], 3);
            break;
    }

    return (IsoMapOutRAmount (pDest, sBuf, nDestLen, sizeof (sBuf), bSign, 0));

} /* End of IsoMapOutRAmountDouble */





