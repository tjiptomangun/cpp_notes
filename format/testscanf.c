/**
 * gcc -Wall -ggdb3 testscanf.c -o testscanf
 **/
#include <stdio.h>
int testscanf (char * label, char * buff)
{
	int prefix = 0;
	int ret = 0;

	ret = sscanf (buff , "%X", &prefix);
	fprintf (stdout, "%s: %X ret : %d\n", label,  prefix, ret);
}
int main (int argc, char **argv)
{
	char * a = "Slurpessssssssss";
	char * b = "10:10:10.123  7A4A  Slurpee";
	char * c = "10:10:10      7A4A  Slurpee";
	char * d = "              7A4A  Slurpee";
	char * e = "                    Slurpee";

	testscanf ("a", a);

	testscanf ("b+12", b+12);

	testscanf ("c+8", c+8);


	testscanf ("d+0", d+0);

	testscanf ("e+0", e+0);

}
