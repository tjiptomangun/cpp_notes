#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int n = 0;
	int r = 0;
	int n_r = 0;
	int fn = 1;
	int fn_r = 1;

	if (argc < 3)
	{
		fprintf (stderr, "usage :%s n r\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	n = atoi (argv[1]);
	r = atoi (argv[2]);
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

	fprintf (stdout ,"fn = %d  fn_r = %d\n", fn, fn_r);
	fprintf (stdout ,"%d\n", fn/fn_r);
	exit (0);

		

		
}
