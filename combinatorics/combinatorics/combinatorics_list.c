#include <stdio.h>
#include <unistd.h>
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

int main (int argc, char **argv)
{
	unsigned long long int n = 0;
	unsigned long long int r = 0;
	unsigned long long int m = 0;
	unsigned long long int res = 0;
	if (argc < 2)
	{
		fprintf (stderr, "usage :%s n \n", argv[0]);
		exit (EXIT_FAILURE);
	} 


	n = atoi (argv[1]);
	fprintf (stdout, "        ");
	for (m = 0; m <=n ; m++)
		fprintf (stdout, "%-8llu", m);
	fprintf (stdout, "\n");
			
	for (m = 0; m <= n ; m++)
	{
		fprintf (stdout, "%-8llu", m);
		for (r = 0; r <= m ; r++)
		{
			res = combinatorics (m, r);
			fprintf (stdout, "%-8llu", res);
		}
		fprintf (stdout, "\n");
	}
	exit (EXIT_SUCCESS);
}
