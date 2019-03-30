
/**
 * bdprdx birthday paradox, combinatorics russell merris 1.20 & 1.21.
 * probability of two person have same birthday/diceside.
 * $CC -Wall -ggdb3 bdprdx2.c -lm -o bdprdx2
 * C(dices expected) * C(sides 1) * (sides ^ (dices - expected)) / sides ^ dices
 **/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


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
/**
 * side_occured
 * number of side 6 appears in exactly expected dices of alldice
 * if all changes (pow (sides, alldice)) are rolled
 */
double side_occured (int side, int alldice, int expected)
{
	int others = alldice - expected;
	double occured = combinatorics (alldice, expected)*pow (side, others);
	return occured ;
}

/**
 * sides_occured
 * number of each side of all available k side of a dice
 * appears in exactly expected dicess of alldice availabale if all
 * changes (pow (alldice, 6)) are rolled
 */
double sides_occured (int sides, int alldice, int expected)
{
	return combinatorics (sides, 1) * 
		side_occured (sides, alldice, expected);
}
/**
 *
 *
 */
double sides_occured2 (int sides, int dices, int expected)
{
	int others = dices - expected;
	unsigned long long int c1 = combinatorics (sides, 1);
	unsigned long long int c2 = combinatorics (dices, expected); /*this comes with intersections, find it*/
	double o1 = pow (sides, others);
	fprintf (stdout, " s = %d\n d = %d\n e = %d\n c1 = %lli\n c2=%lli\n o1=%f\n", 
			sides, dices, expected, c1, c2, o1);
	double occured = combinatorics (sides, 1)*
		pow (sides, (double)others);
	return occured;	
}
int main (int argc, char **argv)
{
	double days = 365.0;
	double deg = 365.0;
	int people= 23;
	double denom , res;
	double accum = 0;
	

	if (argc > 2)
	{
		days= (double) atoi (argv[1]);
		deg = days;
		people = atoi  (argv[2]);
	}

	accum = sides_occured2 ((int) days, people, 2);
	denom = pow (days, people);
	res = accum / denom;
	fprintf (stdout, "accum = %f denom = %f res = %f\n", accum, denom, res);
	//fprintf (stdout, "res = %f\n",  res);
	exit (0);
	
	

}
