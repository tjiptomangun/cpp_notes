/**
 * linux
 * $CC -wall -ggdb3 -D_LE_ endianess.c -o endianess
 * aix
 * $CC -wall -ggdb3 -D_BE_ endianess.c -o endianess
 */
#include <stdio.h> 
#include <stdlib.h>

int main (int argc, char **argv)
{
	unsigned char k[] = {0x1F, 0, 0, 0};
	unsigned char temp = 0;
	unsigned char l[] = {128, 129, 131, 3, 0, 0};
	unsigned char by[8];
	int lenolenmax = 6;
	int length;
	int lenolen;
	int i;
#ifdef _LE_
	temp = k[0];
	k[0] = k[3];
	k[3] = temp;
	temp = k[1];
	k[1] = k[2];
	k[2] = temp; 
#endif

	int rest = *(int *) k;
	fprintf (stdout, "{0x1F,0,0,0} =  %08x\n", rest);
	fprintf (stdout, "{0x1F,0,0,0} =  %08d\n", rest);

	for (i = 0; i < 6; i++)
	{
		if (l[i] & 0x80)
			break;
	}
	//length is i+1;
	if (i<lenolenmax)
		i+=1;
	else
	{
		exit (EXIT_FAILURE);
	}
	if (i<=1)
	{
		by[0] = l[0];	

	}
	else if (i ==2)
	{
		by[0] = l[1];
		by[1] = l[0];
		by[1]&=~0x80;
		by[0]&=~0x80;
	}
	exit (EXIT_SUCCESS);
}
