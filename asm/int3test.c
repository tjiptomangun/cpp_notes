/**
 * int3test : test interrupt 3
 * $CC int3test.c  -Wall -ggdb3 -o int3test
 **/
#   define RT_BREAKPOINT()      __asm__ __volatile__("int $3\n\tnop\n\t")
int main (int argc, char **argv)
{
	RT_BREAKPOINT();
}
/*comment test ed editor*/
/*not that bad*/
