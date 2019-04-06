#include <stdio.h>

int main (int argc, char **argv)
{
	//har *l = "2";
	int k = 2;
	char aku[2];
        sprintf (aku, "%1.1d", k);
	fprintf (stdout, "%c\n", aku[0]);
	exit (0);
}
