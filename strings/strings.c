#include <stdio.h>

int main (int argc, char ** argv)
{
	char *str1 = "Hello World";
	char *str2 = "Jiahhh whatever Hello World we have";

	char * strfind = strstr(str2, str1);
	fprintf (stdout, "ptr = %d \n", strfind);
	if (strfind)
		fprintf (stdout,"%s\n", strfind);	
	exit (0);


}
