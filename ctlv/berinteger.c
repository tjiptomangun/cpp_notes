#include <stdio.h>
#include <inttypes.h>
#include <string.h> 
#include <stdlib.h>
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
#define MAX_NEGATIVE 0x8000000000000000ULL
int xuitlv_intvaltostr (int64_t in, unsigned char *out, int maxout);
int64_t xuitlv_strtointval (unsigned char *in, int inlength);

/**
 * Name		: verifyValue
 * Description	: convert integer to stream and print the result 
 * Input	: integer value 
 * Returns	: TLVALUE_INSUFFBUFF (-8) if buffer is not sufficient
 *		  length of bytes copies if success
 *
 **/
int verifyValue (int64_t in)
{
	unsigned char out[20];
	int ret = 0, i = 0;
	fprintf (stdout, "verifyValue\n");
	memset (out, 0xF, 20);
	ret = xuitlv_intvaltostr (in, out, 20);
	fprintf (stdout , "in = %lld   = %016llx\n", in, in);
	fprintf (stdout , "len = %d    : ", ret);
	for (i = 0; i<ret; i++)
	{
		fprintf (stdout , "%02x",out[i]);
	}
	fprintf (stdout, "\n");
	return ret;
}

/**
 * Name		: verifyValue2
 * Description	: convert integer to stream and print the result 
 * Input	:
 *	in integer value
 *	out output buffer
 *	outsize : output buffer size 
 * Returns	: TLVALUE_INSUFFBUFF (-8) if buffer is not sufficient
 *		  length of bytes copies if success
 *
 **/
int verifyValue2 (int64_t in, unsigned char *out, int outsize)
{ 
	int ret = 0, i = 0;
	fprintf (stdout, "verifyValue\n");
	memset (out, 0xF, outsize);
	ret = xuitlv_intvaltostr (in, out, outsize);
	if (ret < 0)
		return ret;
	fprintf (stdout , "in = %lld   = %016llx\n", in, in);
	fprintf (stdout , "len = %d    : ", ret);
	for (i = 0; i<ret; i++)
	{
		fprintf (stdout , "%02x",out[i]);
	}
	fprintf (stdout, "\n");
	return ret;
}

/**
 * Name		: verifyStr
 * Description	: print stream of len. convert stream to integer and print the result
 *		  
 *
 **/
int verifyStr (unsigned char *in, int len)
{
	fprintf (stdout, "verifyStr\n");
	int64_t val = 0;
	int i = 0;
	for (i = 0; i<len; i++)
	{
		fprintf (stdout , "%02x",in[i]);
	}
	fprintf (stdout, "\n");
	val = xuitlv_strtointval (in , len);
	fprintf (stdout , "val = %lld   = %016llx\n", val, val);
	return 0;	
}
/**
 * Name		: xuitlv_strtointval
 * Description	: convert stream to integer. ber asn.1 dicatates
 * 		  integer presented in big endian order
 * 		  using 2's complement notation.
 **/
int64_t xuitlv_strtointval (unsigned char *in, int inlength)
{
	int64_t ret = 0;
	int i =0;
	unsigned int rest = 0;
	for (i = 0; i<inlength; i++)
	{
		ret<<=8;
		ret|= in[i];
	} 
	if (in[0] & 0x80)
	{
		rest = 16 - inlength;
		rest<<=3;
		ret<<=rest;
		ret>>=rest; 
	}
	return ret;
	
}
/**
 * Name		: xuitlv_intvaltostr
 * Description	: convert integer to stream. ber asn.1 dictates
 * 		  integer presented in big endian order 
 * 		  using 2's complement notation.  
 * Returns	
 *	TLVALUE_INSUFFBUFF (-8)	if buffer is not sufficient
 *	length of bytes copied if success
 **/
int xuitlv_intvaltostr (int64_t in, unsigned char *out, int maxout)
{
	int ret = 0;
	int i = 0;
	int halfbyte  = 0; 
	int64_t i_test;
	uint64_t u_test; 
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
			out[ret-1-i] = ctemp;
		}
		return ret;
	}
	else
	{ 
		i_test = ~in + 1; 
		u_test = (uint64_t) i_test; 
		while ((++i <= maxout) && (u_test>=0x100)) 
			u_test>>=8; 
		if (i >= maxout)
			return TLVALUE_INSUFFBUFF; 
		if ((u_test & 0x80) && (in!=MAX_NEGATIVE)) 
		{
			i++;
			if (i>=maxout)
				return TLVALUE_INSUFFBUFF; 
		}
		ret = i; 
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
	unsigned char k[20];
	int ret =0;
	if (argc <2)
	{
		fprintf (stdout, "usage %s integer_value\n", argv[0]);
		exit (EXIT_SUCCESS);
	}
	//verifyValue (1);
	memset (k, 0, 20);
	ret = verifyValue2 (0x8000000000000000ULL, k, 20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0x8000000000000001ULL, k, 20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0xFFFF000000000001ULL,k ,20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0xFFFFFF0000000001ULL,k ,20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0xFFFFFFFF00000001ULL,k ,20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0xFFFFFFFFFFFFFFFFULL,k, 20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (-0xFF,k ,20);
	if (ret>0)	
		verifyStr(k,ret);

	ret = verifyValue2 (-0x100,k ,20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0xFFFF9EFF000000FFULL,k ,20);
	if (ret>0)
		verifyStr(k,ret);

	ret = verifyValue2 (0x0,k ,20);
	if (ret>0)
		verifyStr(k,ret);
		

	ret = verifyValue2 (0xFFFFFFFF000000FFULL,k ,20);
	if (ret>0)
		verifyStr(k,ret);
	
	ret = verifyValue2 (255,k ,20);
	if (ret>0)
		verifyStr(k,ret);
	



	/*
	ret = verifyValue2 (0x0080000000000001 ,20);
	ret = verifyValue2 (0x000080D0C0B05031);
	ret = verifyValue2 (0x000080);
	*/
	ret = verifyValue2 (357,k ,20);
	if (ret>0)
		verifyStr(k,ret);
	
//	ret = verifyValue2 (0x1);
//	ret = verifyValue2 (0x2);
	//valin = atoll (argv[1]);
	//verifyValue (valin);
	exit (EXIT_SUCCESS);

}
