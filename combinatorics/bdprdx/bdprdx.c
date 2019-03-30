/**
 * bdprdx birthday paradox, combinatorics russell merris 1.20 & 1.21
 * probability of two person have same birthday
 * 1 - none_the_same
 * $CC -Wall -ggdb3 bdprdx.c -lm -o bdprdx
 **/
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

unsigned long long int factorial (int r)
{
	if (r > 1)
		return r * factorial (r - 1);
	return 1;
}
	
unsigned long long int combinatorics(int n, int r)
{

	unsigned long long int n_r = 0;
	unsigned long long int fn = 1;
	unsigned long long int fn_r = 1;

	n_r = n - r;
	r = r > n_r ? r : n_r;
	n_r = n - r; 

	for (; n > r ; n--)
	{
		fn *= n;
	}
	for (; n_r >0; n_r--)
	{
		fn_r *= n_r;
	}

	return fn/ fn_r;
}
int main (int argc, char **argv)
{
	double fact = 365.0;
	double deg = 365;
	int idx = 23;
	double denom, res;
	double accum = 1.0;

	if (argc >2)
	{
		fact = (double)atoi (argv[1]);
		deg = fact;
		idx = atoi (argv[2]);
	}

	denom = pow (fact, idx);
	for ( ; idx>0 ; idx --)
	{
		accum*=deg;
		deg--;
	}
	res = (double)accum;

	res /= denom;
	res = 1.0 - res;
	fprintf (stdout , "res = %f\n", res);
	exit (0);

}
