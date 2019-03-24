#include <stdio.h>
#include <inttypes.h>
#include <string.h>
/**
 * $CC -Wall -ggdb3 berinteger.c -o berinteger
 * Name		: xuitlv_intvaltostr
 * Description	: transform integer value into tlv array
 * Input	
 *	in	: integer value to transform
 *	out	: buffer to store value
 * Returns
 *	-1	: error transforming
 *	>=0	: length of copied data to buffer
 **/
#define TLVALUE_INSUFFBUFF	-8 
int verifyValue (int64_t in)
{
	unsigned char out[20];
	int ret = 0, i = 0;
	memset (out, 0, 20);
	ret = xuitlv_intvaltostr (in, out, 20);
	fprintf (stdout , "in = %030lld   = %020llx\n", in, in);
	fprintf (stdout , "len = %d    : ", ret);
	for (i = 0; i<ret; i++)
	{
		fprintf (stdout , "%02x",out[i]);
	}
	fprintf (stdout, "\n");
	return ret;
}
int xuitlv_intvaltostr (int64_t in, unsigned char *out, int maxout)
{
	int ret = 0;
	int i = 0;
	int halfbyte  = 0; 
	int64_t i_test;
	unsigned char ctemp = 0x00; 
	
	if (in>=0)
	{ 
		do
		{
			out[i++] = (in & 0xFF);
			in >>= 8; 
		} while ((in > 0) && (i <= maxout));
		if (i > maxout)
			return TLVALUE_INSUFFBUFF;
		if (out[i-1] & 0x80)
		{
			if (i >= maxout)
				return TLVALUE_INSUFFBUFF;
			out[i++] = 0x00; 
		}
		ret = i;
		halfbyte = ret>>1;
		for (i = 0; i<halfbyte; i++)
		{
			ctemp = out[i];
			out[i] = out [ret-1-i];
		}
		return ret;
	}
	else
	{
		i_test = in;
		in = ~i_test;
		in+=1; 
		do
		{ 
			ctemp = in &0xFF;
			in >>= 8; 
			i++;
		} while ((in > 0) && (i <= maxout));
		if (i > maxout)
			return TLVALUE_INSUFFBUFF;
		if (ctemp & 0x80) 
		{
			if (i>=maxout)
				return TLVALUE_INSUFFBUFF;
			i++;
		}
		ret = i;
		in = i_test;
		for (i = 0; i < ret; i++)
		{
			out[ret-i-1] = (in&0xFF);
			in>>=8;
		}
		return ret;
	}
}
int main (int argc, char **argv)
{
	int64_t valin = 0;
	int64_t valout = 0;
	unsigned char arrin [20];
	unsigned char arrout [20]; 
	verifyValue (1);
	verifyValue (2); 
	verifyValue (-1);
	verifyValue (200);
	verifyValue (-2); 
	verifyValue (0xFF00000000000000ULL); 
	verifyValue (0x8000000000000000ULL); 

}
