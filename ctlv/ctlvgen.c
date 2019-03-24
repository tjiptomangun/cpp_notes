/*
 * $CC ctlvgen.c -Wall -ggdb3 -o ctlvgen
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>


#define CTLV_NOMORETAG		0
#define CTLV_NOVALIDTAG		-1
#define CTLV_OUTOFMEMORY	-2
#define CTLV_NULLROOT		-3
#define CTLV_TAGNOTFOUND	-4
#define CTLV_NOPRIMITIVETAG	-5
#define CTLV_UNSUFFICIENTBUFFSIZE -6
#define CTLV_INVALIDTLVSTREAMLENGTH	-7
#define CTLV_INVALIDSTREAMLENGTH -8

#define maxtaglen 20
#define maxdata   40
#define maxtlvlen    10
#define MAX_SINGLE_TAGLEN 10

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

 
/**
 * Name		: getNextTag
 * Description	: getNextTag in this stream
 * Input
 *	in	: stream to evaluate
 *	maxlen	: maximum length
 * Returns
 *	length of tag in this in, started from index 0.
 *	if it is started with 0x00 then no more tag, returns 0.
 *	if after maxlen there are no unexpanded tag found then 
 *	it returns 0.  
 */

int getNextTag (unsigned char * in, int maxlen)
{
	int i = 0;
	if (in[i] == 0x00)
		return CTLV_NOMORETAG;
	while ((in[i] & 0x80)&& (i < maxlen))
	{
		i++;
	}
	if (i == maxlen)
		return CTLV_NOVALIDTAG; 
	return i+1; 
}

/**
 * Name		: tagCmp
 * Description	: compare to tag. it is an MSB (little endian compare).  
 * Input
 *	tagA	: first tag
 *	tagB	: second tag
 * Returns
 *	>0	: if tagA  > tagB, also if length tagA > length tagB
 *      <0	: if tagA  < tagB
 *	0	: if both equals
 **/
int tagCmp (unsigned char *tagA, unsigned char *tagB)
{
	int lenA = 0;
	int lenB = 0;
	lenA = getNextTag (tagA,16);
	lenB = getNextTag (tagB,16);
	if (lenA != lenB)
	{
		return (lenA - lenB);
	}
	return memcmp (tagA, tagB, lenA);	
}

/**
 * Name		: getTlvStreamDataLength
 * Description	: get length of this data
 * Input 
 *	in	: stream to evaluate
 *	maxbyte	: maximum byte to evaluate
 *	result	: result length
 * Returns
 *	length of length  in bytes
 */
int getTlvStreamDataLength (unsigned char * in, int maxlength,   int64_t* result)
{ 
	int64_t res = 0;
	int i = 0; 
	res |= (in[i]&0x7F) ;
	i++;
	while ( (in[i-1] & 0x80))
	{
		res <<=7; 
		res |= (in[i]&0x7F);
		i++;
	}
	*result = res;
	return i;

}
/**
 * Name		: setTlvStreamDataLength
 * Description	: set tlv length, base 128
 * Input
 *		in 	: values to set in tlv
 *		maxout 	: maxoutput buffer size
 * Output
 *		out 	: array to put the tlv length
 * Returns	: number of bytes written to out
 **/

int setTlvStreamDataLength (int64_t in, unsigned char *out, int maxout)
{ 
	int i = 0;
	int maxbytes = 0;
	int halfbyte = 0; 
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
	halfbyte = maxbytes>>1;
	for (i=0; i<halfbyte; i++)
	{
		ctemp = out[i];
		out[i] = out[maxbytes-1-i];
		out[maxbytes-1-i] = ctemp;
	} 

	return maxbytes;
}
/**
 * Name		: calculateTlvLengthUsed
 * Description	: count number of bytes will be occupied if length is =val
 * Returns	: return the counted value. max value is 9 bytes
 **/
int calculateTlvLengthUsed (int64_t val)
{ 
	int ret = 1;
	if (val >=0x80)
		ret+=1; 
	if (val >=0x4000)
		ret+=1; 
	if (val >=0x200000)
		ret+=1; 
	if (val >=0x10000000)
		ret+=1; 
	if (val >= 34359738368LL)
		ret+=1; 
	if (val >= 4398046511104LL)
		ret+=1; 
	if (val >= 562949953421312LL)
		ret+=1;
	if (val >= 72057594037927936LL)
		ret+=1;
	return ret;
}

/**
 * Name		: assignTag
 * Description	: assign a new tag to destination tag
 **/
int assignTag (unsigned char *dest, unsigned char *src)
{
	int srcLen = getNextTag(src, MAX_SINGLE_TAGLEN); 
	memcpy (dest, src, srcLen);
	return srcLen;


}


typedef struct ctlvgenstr
{
	unsigned char tag[maxtaglen]; 
	int vallen; 
	unsigned char data[maxdata];
	int datalen;
	struct ctlvgenstr *next; 
}CTLVGENSTR, *PCTLVGENSTR;

PCTLVGENSTR newctlvgen (char *tag, int vallen, char *data)
{
	PCTLVGENSTR gen = (PCTLVGENSTR) calloc (1, sizeof (CTLVGENSTR));	
	int strtaglen = strlen(tag);
	int strdatalen = 0;
	if (!gen)
		return gen;
	if (!strtaglen)
		return NULL;
	strtaglen>>=1;
	if (strtaglen > maxtaglen )
		strtaglen = maxtaglen;
	HtoB (gen->tag, tag, strtaglen*2); 
	gen->datalen = 0;
	gen->vallen = vallen;
	if (!data)
		return gen; 
	strdatalen = strlen (data);
	gen->datalen = (strdatalen>>1);
	HtoB (gen->data, data, strdatalen);
	return gen;
}

PCTLVGENSTR addctlvgen (PCTLVGENSTR root, PCTLVGENSTR added)
{
	PCTLVGENSTR curr;
	if (!root)
		return added;
	curr = root;
	while (curr->next)
		curr = curr->next;
	curr->next = added;	
	return root; 
}

void printtlvgenstr (PCTLVGENSTR p)
{
	int i = 0;
	int n = 0;
	int taglen = getNextTag (p->tag,maxtaglen);
	unsigned char buffer[maxtlvlen];
	for (i=0;i<taglen; i++) 
		fprintf (stdout, "%02X", p->tag[i]); 
	
	n = setTlvStreamDataLength (p->vallen, buffer, maxtlvlen);
	for (i=0;i<n; i++)
		fprintf (stdout, "%02X", buffer[i]);
	if (!p->datalen)
		return;
	for (i=0; i<p->datalen; i++)
		fprintf (stdout, "%02X", p->data[i]);		
}

int increasetag(PCTLVGENSTR p)
{
	int taglen = getNextTag (p->tag, maxtaglen);
	
	if (!taglen)
		return 0;
	p->tag[taglen-1]++;
	return 0;
}
int increaselisttag (PCTLVGENSTR root)
{
	PCTLVGENSTR curr = root;
	while (curr)
	{
		increasetag (curr);
		curr = curr->next;
	}
	return 0;
}

int printlisttag (PCTLVGENSTR root)
{
	PCTLVGENSTR curr = root;
	while (curr)
	{
		printtlvgenstr(curr); 
		curr = curr->next;
	}
	return 0;
}
int main (int argc, char **argv)
{
	PCTLVGENSTR root = addctlvgen (NULL, newctlvgen ("D0C701",0x5F, NULL)); 

	int i=0;
	root = addctlvgen (root, newctlvgen ("8006", 0x0D, "DA7A0101FACEB00C789ABCDEF0"));
	root = addctlvgen (root, newctlvgen ("FA11", 0x23, NULL));
	root = addctlvgen (root, newctlvgen ("BAD501", 0x0C, "DA7A0102FACEB00C789ABCDE"));
	root = addctlvgen (root, newctlvgen ("FAC1A151", 0x0E, NULL));
	root = addctlvgen (root, newctlvgen ("A111", 0x0B, "DA7A0103FACEB00C789ABC"));
	root = addctlvgen (root, newctlvgen ("FEEDDADEADF151", 0x21, NULL));
	root = addctlvgen (root, newctlvgen ("ABADFEED01", 0x0A, "DA7A0104FACEB00C789A"));
	root = addctlvgen (root, newctlvgen ("C1CC01", 0x0D, NULL));
	root = addctlvgen (root, newctlvgen ("BABE01", 0x09, "DA7A0105FACEB00C78"));
		

	do
	{
		printlisttag (root);
		increaselisttag (root); 
		i++;
	}while (i<10);
	fprintf (stdout, "\n");
	exit (EXIT_SUCCESS);
}
