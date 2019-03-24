#include <stdio.h>
#include <inttypes.h>


/**
 * Name         : getArrayStreamDataLength
 * Descriptioni : get length of this data
 * Input
 *      in      : stream to evaluate
 *      maxbyte : maximum bytes to evaluate
 *      result  : result length
 * Returns
 *      length of length in bytes
 *
 */
int getArrayStreamDataLength (unsigned char *in, int maxlength, int64_t *result)
{
        int64_t res = 0;
        int i = 0;
        i = 3;
        do
        {
                res <<=8;
                res+= (int64_t)in[i];
                i--;
        }while (i >= 0);
	*result = res;
}

/**
 * Name         : setArrayStreamDataLength
 * Description  : set tlv length of array structure
 *
 **/
int setArrayStreamDataLength (int64_t in, unsigned char *out)
{
        int i = 0;
        for (i = 0; i < 4; i++)
        {
                out[i] = in & 0x0FFL;
                in >>=8;
        }
        return 4;
}


int main (int argc, char **argv)
{
	int64_t result1, result2;
	unsigned char k [5] = {0x10,0x11,0x00, 0x89, 0};
	unsigned char j [5] = {0x98,0x11,0x00, 0x89, 9};
	int i = 0;

	unsigned char out [16] ;

	getArrayStreamDataLength (k, 4, &result1); 
	fprintf (stdout, "%012X", result1);
	fprintf (stdout, "\n");
	getArrayStreamDataLength (j, 4, &result2);
	fprintf (stdout, "%012X", result2); 
	fprintf (stdout, "\n");

	i = setArrayStreamDataLength (result1, out);
	while ((i-1)>=0)
	{
		fprintf (stdout, "%02X", out[i-1]);
		i--;
	} 
	fprintf (stdout, "\n");

	i = setArrayStreamDataLength (result2, out);
	while ((i-1)>=0)
	{
		fprintf (stdout, "%02X", out[i-1]);
		i--;
	} 
	fprintf (stdout, "\n");

	
	
}
