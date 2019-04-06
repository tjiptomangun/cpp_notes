#include <stdio.h>

int main (int argc, char **argv)
{
	int k = 101;
	int l = 10;
	k-= l +1;
	fprintf (stdout, "k = %d\n", k);
	exit (0);

}
