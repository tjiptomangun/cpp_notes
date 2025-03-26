#include <stdio.h>

int main (int argc, char ** argv)
{
	char c1[100] = {0};
	char *str1 = "Hello World";
	char *str2 = "Jiahhh whatever Hello World we have";

	char * strfind = strstr(str2, str1);
	fprintf (stdout, "ptr = %d \n", strfind);
	if (strfind)
		fprintf (stdout,"%s\n", strfind);	
	sprintf(c1, "%s*", c1);
	sprintf(c1, "%s*", c1);
	fprintf (stdout,"%s\n", c1);	
	exit (0);


}
