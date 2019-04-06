#include<stdio.h>

int main ( int argc, char **argv )
{
	char  str1[80];
	int nInt1; 
	printf ("type string %%10s_%%4d : ");
	scanf ("%10s_%4d",str1, &nInt1 );
	printf ("you type %s %d\n", str1, nInt1);
	return 0;

}
