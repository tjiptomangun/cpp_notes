#include <stdio.h>

int main (int argc, char **arg)
{
	int i = 0;
	while (1)
	{
		if (!(i%10000))
		{
			fprintf (stdout,"-----------------");
			i++;
			continue;
		}
		fprintf (stdout, (i%100)?"%05d ":"%05d\n\n\n", i);
		i++;
	} 
}
