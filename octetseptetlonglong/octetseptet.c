/**
 * linux :
 * $CC -Wall -ggdb3 octetseptet.c -D_LE_ -lm -o octetseptet
 *
 * aix: 
 * $CC -Wall -ggdb3 octetseptet.c -D_BE_ -lm -o octetseptet
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

void alarmHandler(int signum)
{
	cont = 0;
}
int main (int argc, char **argv)
{
	long long int res = 0;
	int max =0;
	unsigned char k_1[] = {0x01, 0x00};//1
//	unsigned char k_1[] = {0xAF, 0x11};
	unsigned char k_2[] = {0xAF, 0x11, 0x00};//135
	unsigned char k_3[] = {0x81, 0x91, 0x7F};//18687
	unsigned char k_4[] = {0x81, 0x80, 0x82, 0x01};//2097409
	unsigned char k_5[] = {0x81, 0x80, 0x80, 0x80, 0x00,0x00};//268435456
	unsigned char k_6[] = {0x81, 0x80, 0x80, 0x80, 0x82, 0x01};//34359738625
	unsigned char k_7[] = {0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00};//4398046511104
	unsigned char k_8[] = {0x81, 0x80, 0x80, 0x80, 0x80, 0x83, 0x82, 0x00};//562949953470720
	unsigned char k_9[] = {0x81, 0x80, 0x80, 0x82, 0x80, 0x80, 0x80, 0x80, 0x00};//72057662757404672 

	unsigned char buffer[12]; 
	memset (buffer, 0, 12);
	
	
	fprintf (stdout,"============getlong==================\n");
	
	res = gettlvlength (k_1, 1);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_2, 3);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_3, 3);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_4, 4);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_5, 6);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_6, 6);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_7, 7);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_8, 8);
	fprintf (stdout, "ret : %lld\n", res);
	res = gettlvlength (k_9, 9);
	fprintf (stdout, "ret : %lld\n", res);

	//int settlvlength (long long int in, unsigned char *out, int maxout)
	fprintf (stdout,"============setlong==================\n");
	printTlvLength (1, buffer, 12);
	printTlvLength (6033, buffer, 12);
	printTlvLength (18687, buffer, 12);
	printTlvLength (2097409, buffer, 12);
	printTlvLength (268435456, buffer, 12);
 	printTlvLength ((long long int)34359738625LL, buffer, 12);
	printTlvLength ((long long int)4398046511104LL, buffer, 12);
	printTlvLength ((long long int)562949953470720LL, buffer, 12);
	printTlvLength ((long long int)72057662757404672LL, buffer, 12);
	printTlvLength ((long long int)72057662757404672LL, buffer, 5);


	fprintf (stdout,"============get64==================\n");
	res = int_gettlvlength (k_1, 2);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_2, 3);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_3, 3);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_4, 4);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_5, 6);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_6, 6);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_7, 7);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_8, 8);
	fprintf (stdout, "ret : %lld\n", res);
	res = int_gettlvlength (k_9, 9);
	fprintf (stdout, "ret : %lld\n", res);

	
	fprintf (stdout,"============set64==================\n");
	int_printTlvLength (1, buffer, 12);
	int_printTlvLength (6033, buffer, 12);
	int_printTlvLength (18687, buffer, 12);
	int_printTlvLength (2097409, buffer, 12);
	int_printTlvLength (268435456, buffer, 12);
 	int_printTlvLength ((long long int)34359738625LL, buffer, 12);
	int_printTlvLength ((long long int)4398046511104LL, buffer, 12);
	int_printTlvLength ((long long int)562949953470720LL, buffer, 12);
	int_printTlvLength ((long long int)72057662757404672LL, buffer, 12);
	int_printTlvLength ((long long int)72057662757404672LL, buffer, 5);


	signal (SIGALRM, alarmHandler);	
	
	cont = 1;
	j = 0;
	alarm (1);
	do
	{ 
		res = gettlvlength (k_1, 1); 
		res = gettlvlength (k_2, 3); 
		res = gettlvlength (k_3, 3); 
		res = gettlvlength (k_4, 4); 
		res = gettlvlength (k_5, 6); 
		res = gettlvlength (k_6, 6); 
		res = gettlvlength (k_7, 7); 
		res = gettlvlength (k_8, 8); 
		res = gettlvlength (k_9, 9); 
		max = settlvlength (1, buffer, 12);
		max = settlvlength (6033, buffer, 12);
		max = settlvlength (18687, buffer, 12);
		max = settlvlength (2097409, buffer, 12);
		max = settlvlength (268435456, buffer, 12);
		max = settlvlength ((long long int)34359738625LL, buffer, 12);
		max = settlvlength ((long long int)4398046511104LL, buffer, 12);
		max = settlvlength ((long long int)562949953470720LL, buffer, 12);
		max = settlvlength ((long long int)72057662757404672LL, buffer, 12);
		max = settlvlength ((long long int)72057662757404672LL, buffer, 12);
		j++;
	
	}
	while(cont);



	fprintf (stdout, "numops1 =%d\n", j);



	cont = 1;
	j = 0;


	alarm (1);
	do
	{

		res = int_gettlvlength (k_1, 1); 
		res = int_gettlvlength (k_2, 3); 
		res = int_gettlvlength (k_3, 3); 
		res = int_gettlvlength (k_4, 4); 
		res = int_gettlvlength (k_5, 6); 
		res = int_gettlvlength (k_6, 6); 
		res = int_gettlvlength (k_7, 7); 
		res = int_gettlvlength (k_8, 8); 
		res = int_gettlvlength (k_9, 9); 
		max = int_settlvlength (1, buffer, 12);
		max = int_settlvlength (6033, buffer, 12);
		max = int_settlvlength (18687, buffer, 12);
		max = int_settlvlength (2097409, buffer, 12);
		max = int_settlvlength (268435456, buffer, 12);
		max = int_settlvlength ((long long int)34359738625LL, buffer, 12);
		max = int_settlvlength ((long long int)4398046511104LL, buffer, 12);
		max = int_settlvlength ((long long int)562949953470720LL, buffer, 12);
		max = int_settlvlength ((long long int)72057662757404672LL, buffer, 12);
		j++;
	}while(cont);

	fprintf (stdout, "numops2 =%d\n", j);
	exit (EXIT_SUCCESS);
}
