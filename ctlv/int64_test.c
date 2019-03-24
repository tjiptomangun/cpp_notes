/**
 * $CC int64_test.c -Wall -ggdb3 -o int64_test
 **/
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
int varifyNegativeBytes2(int64_t in)
{

}
int verifyNegativeBytes (int64_t in)
{
	int i;
	int64_t temp;
	fprintf (stdout ,"in = %llx = %lld\n", in, in);
	i = 0;

	while (in && ((in & 0xFF00000000000000) == 0xFF00000000000000) && (i<7))
	{
		i++;
		in <<= 8;	
	} 

	temp = in;
	
	
	
	fprintf (stdout , "i = %d; used  = %d\n", i, 8-i);
}
int main (int argc, char **argv)
{
	int i = 0;
	int64_t minOne = -1;
	int64_t minTwo = -2;

	int64_t minBigOne = -9223372036854775806LL;
	int64_t minBigTwo = -9223372036854775708LL; 
	uint64_t u64_1 = 0xFFFFF0000000FE56ULL;
	uint64_t u64_2 = 0x00FFF0000000FE56ULL;
	uint64_t u64_3 = 0x0000FF000000FE56ULL;
	int64_t i64_1 = 0;
	int64_t i64_2 = 0;
	int64_t i64_3 = 0;
	i64_1|= u64_1;
	fprintf (stdout ,"i64_1 = %llx = %lld\n", i64_1, i64_1); 
	i64_1 = ~i64_1;
	i64_1+=1;
	fprintf (stdout ,"i64_1 = %llx = %lld\n", i64_1, i64_1);
	verifyNegativeBytes (minOne);
	verifyNegativeBytes (minTwo); 
	verifyNegativeBytes (0xFF00000000000000ULL); 
	verifyNegativeBytes (0x8000000000000000ULL); 
	
}

