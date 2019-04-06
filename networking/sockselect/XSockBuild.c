#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
/** 
 * The _xtoa() function converts the integer value into the equivalent
 * string in base radix notation, considering the negative sign,
 * storing the result in the specified buffer. The function terminates
 * the string with a NULL character.
 * 
 * @param value     The value to convert into a string.
 * @param buffer    A buffer in which the function stores the string.
 *					The size of the buffer must be at least 33 bytes
 *					when converting values in base 2 (binary).
 * @param radix		The base to use when converting the number. This
 *					value must be in the range: 2 <= radix <= 36
 *					If the value of radix is 10, and value is negative,
 *					then a minus sign is prepended to the result.
 * @param negative	Whether the value is negative or not.
 * @return		  	nothing.
 */
static void _xtoa(unsigned long value, char *buffer, unsigned radix, int negative) {
	char *p;
	char *firstdig;
	char temp;
	unsigned digval;

	p = buffer;

	if (negative) {
		/* Negative, so output '-' and negate */
		*p++ = '-';
		value = (unsigned long)(-(long) value);
	}

	/* Save pointer to first digit */
	firstdig = p;

	do {
		digval = (unsigned) (value % radix);
		value /= radix;

		/* Convert to ascii and store */
		if (digval > 9) {
			*p++ = (char) (digval - 10 + 'a');
		} else {
			*p++ = (char) (digval + '0');
		}
	} while (value > 0);

	/**
	 * We now have the digit of the number in the buffer, but in
	 * reverse order. Thus we reverse them now.
	 */

	*p-- = '\0';
	do {
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;
		p--;
		firstdig++;
	} while (firstdig < p);
}
/** 
 * The _itoa() function converts the integer value into the equivalent
 * string in base radix notation, storing the result in the specified
 * buffer. The function terminates the string with a NULL character.
 * 
 * @param value     The value to convert into a string.
 * @param buffer    A buffer in which the function stores the string.
 *					The size of the buffer must be at least:
 *						8 x sizeof(int) + 1
 *					bytes when converting values in base 2 (binary).
 * @param radix		The base to use when converting the number. This
 *					value must be in the range: 2 <= radix <= 36
 *					If the value of radix is 10, and value is negative,
 *					then a minus sign is prepended to the result.
 * @return		  	A pointer to the resulting string.
 */
char *_itoa(int value, char *buffer, int radix) {
	if (radix == 10 && value < 0) {
		_xtoa((unsigned long) value, buffer, radix, 1);
	} else {
		_xtoa((unsigned long)(unsigned int) value, buffer, radix, 0);
	}

	return buffer;
}
#define		itoa		_itoa
char XSockChecksum (char *pBuf, int nLen)
{
	int				n;
	char			cChecksum = 0;
	
	for (n = 0; n < nLen; ++n)
	{
		cChecksum ^= pBuf[n];
	}
	
	return (cChecksum);
	
} /* End of XSockChecksum */
 

extern void XSockBuildHeaderLenNone ();
extern void XSockBuildHeaderLenBin1 (char *pData, int nLen);
extern void XSockBuildHeaderLenBin2 (char *pData, int nLen);
extern void XSockBuildHeaderLenBin4 (char *pData, int nLen);
extern void XSockBuildHeaderLenNetS (char *pData, int nLen);
extern void XSockBuildHeaderLenNetL (char *pData, int nLen);
extern void XSockBuildHeaderLenDec1 (char *pData, int nLen);
extern void XSockBuildHeaderLenDec2 (char *pData, int nLen);
extern void XSockBuildHeaderLenDec3 (char *pData, int nLen);
extern void XSockBuildHeaderLenDec4 (char *pData, int nLen);
extern void XSockBuildHeaderLenHexDec2 (char *pData, int nLen);
extern void XSockBuildHeaderLenHexDec4 (char *pData, int nLen);
extern void XSockBuildHeaderLenAsciiHex2 (char *pData, int nLen);
extern void XSockBuildHeaderLenAsciiHex4 (char *pData, int nLen);
extern void XSockBuildHeaderLenFlexi (char *pData, int nLen);
extern void XSockBuildHeaderCustomBukopin (char *pData, int nLen);
extern void XSockBuildChecksumNone ();
extern void XSockBuildChecksumBin (char *pChecksum, char *pBuf, int nLen);
extern void XSockBuildChecksumHex (char *pChecksum, char *pBuf, int nLen);


void (*XSockBuildHeaderLen[])() =
{
/*  0 */	XSockBuildHeaderLenNone,
/*  1 */	XSockBuildHeaderLenBin1,
/*  2 */	XSockBuildHeaderLenBin2,
		XSockBuildHeaderLenBin4,
		XSockBuildHeaderLenNetS,
		XSockBuildHeaderLenNetL,
		XSockBuildHeaderLenDec1,
		XSockBuildHeaderLenDec2,
		XSockBuildHeaderLenDec3,
		XSockBuildHeaderLenHexDec2,
/* 10 */	XSockBuildHeaderLenHexDec4,
		XSockBuildHeaderLenAsciiHex2,
		XSockBuildHeaderLenAsciiHex4,
		XSockBuildHeaderLenDec4,
		XSockBuildHeaderLenFlexi,
/* 15 */	XSockBuildHeaderCustomBukopin
};


void (*XSockBuildChecksum[])() =
{
	XSockBuildChecksumNone,
	XSockBuildChecksumBin,
	XSockBuildChecksumHex
	
};



void XSockBuildHeaderLenNone ()
{
	return;
}


void XSockBuildHeaderLenBin1 (char *pData, int nLen)
{
	memcpy (pData, (char *) &nLen, 1);
			
} /* End of XSockBuildHeaderLenBin1 */



void XSockBuildHeaderLenBin2 (char *pData, int nLen)
{
#ifdef _BE_
	int nTmp = nLen;
	nLen = (nTmp << 24) & 0xFF000000;
	nLen |= (nTmp << 8) & 0x00FF0000;
	memcpy (pData, (char *) &(nLen), 2);
#else
	memcpy (pData, (char *) &nLen, 2);
#endif
			
} /* End of XSockBuildHeaderLenBin2 */



void XSockBuildHeaderLenBin4 (char *pData, int nLen)
{
	memcpy (pData, (char *) &nLen, 4);
			
} /* End of XSockBuildHeaderLenBin4 */



void XSockBuildHeaderLenNetS (char *pData, int nLen)
{
	unsigned short		*pInt16 = (unsigned short *) pData;
	
	*pInt16 = htons ((unsigned short) nLen);

} /* End of XSockBuildHeaderLenNetS */



void XSockBuildHeaderLenNetL (char *pData, int nLen)
{
	unsigned long 		*pInt32 = (unsigned long *) pData;

	*pInt32 = htonl (nLen);

} /* End of XSockBuildHeaderLenNetL */



void XSockBuildHeaderLenDec1 (char *pData, int nLen)
{
	char				sTemp[10];
	
	itoa (nLen, sTemp, 10);
	*pData = sTemp[0];

} /* End of XSockBuildHeaderLenDec1 */



void XSockBuildHeaderLenDec2 (char *pData, int nLen)
{
	char				sTemp[10];

	sprintf (sTemp, "%2.2i", nLen);	
	memcpy (pData, sTemp, 2);

} /* End of XSockBuildHeaderLenDec2 */



void XSockBuildHeaderLenDec3 (char *pData, int nLen)
{
	char				sTemp[10];
	
	sprintf (sTemp, "%3.3i", nLen);
	memcpy (pData, sTemp, 3);

} /* End of XSockBuildHeaderLenDec3 */



void XSockBuildHeaderLenDec4 (char *pData, int nLen)
{
	char				sTemp[10];
	
	sprintf (sTemp, "%4.4i", nLen);
	memcpy (pData, sTemp, 4);

} /* End of XSockBuildHeaderLenDec4 */



void XSockBuildHeaderLenHexDec2 (char *pData, int nLen)
{
	*pData = ((nLen / 10) << 4) + ((nLen - (nLen / 10) * 10));

} /* End of XSockBuildHeaderLenHexDec2 */



void XSockBuildHeaderLenHexDec4 (char *pData, int nLen)
{
	char		sTemp[10];
	
	sprintf (sTemp, "%4.4i", nLen);
	pData[0] = ((sTemp[0] - '0') << 4) + (sTemp[1] - '0');
	pData[1] = ((sTemp[2] - '0') << 4) + (sTemp[3] - '0');

} /* End of XSockBuildHeaderLenHexDec4 */



void XSockBuildHeaderLenAsciiHex2 (char *pData, int nLen)
{
	char				sTemp[10];
	
	sprintf (sTemp, "%2.2X", nLen);
	memcpy (pData, sTemp, 2);

} /* End of XSockBuildHeaderLenAsciiHex2 */



void XSockBuildHeaderLenAsciiHex4 (char *pData, int nLen)
{
	char				sTemp[10];
	
	sprintf (sTemp, "%4.4X", nLen);
	memcpy (pData, sTemp, 4);

} /* End of XSockBuildHeaderLenAsciiHex4 */




void XSockBuildHeaderLenFlexi (char *pData, int nLen)
{
	if (nLen > 128)
	{
		pData[0] = (char) 194;		/* 0xC2 */
		memcpy (&pData[1], (char *) &nLen, 2);
	}
	else
	{
		memcpy (pData, (char *) &nLen, 2);
	}

} /* End of XSockBuildHeaderLenFlexi */






void XSockBuildChecksumNone ()
{
	return;
	
} /* End of XSockBuildChecksumNone */



void XSockBuildChecksumBin (char *pChecksum, char *pBuf, int nLen)
{
	*pChecksum = XSockChecksum (pBuf, nLen);

} /* End of XSockBuildChecksumBin */



void XSockBuildChecksumHex (char *pChecksum, char *pBuf, int nLen)
{
	char			sTemp[3];
	char			cTemp;
	
	cTemp = XSockChecksum (pBuf, nLen);
	sprintf (sTemp, "%2.2X", (int) cTemp);
	memcpy (pChecksum, sTemp, 2);
	
} /* End of XSockBuildChecksumHex */


void XSockBuildHeaderCustomBukopin (char *pData, int nLen)
{
	// Custom Bukopin
	char				sTemp[10];
	
	sprintf (sTemp, "%4.4i", nLen-4);
	memcpy (pData, sTemp, 4);

} /* End of XSockBuildHeaderLenDec4 */
