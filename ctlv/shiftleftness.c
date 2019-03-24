/**
 * $CC -Wall -ggdb3 shiftleftness.c -o shiftleftness
 **/
#include <stdio.h>
#include <inttypes.h>

int main (int argc, char **argv)
{
	int64_t k = 0x80;
	fprintf (stdout ,"k = %lld ; %016llx\n", k , k );
	k <<= 56;
	fprintf (stdout ,"k = %lld ; %016llx\n", k , k );
	k >>= 56;
	fprintf (stdout ,"k = %lld ; %016llx\n", k , k );

}
