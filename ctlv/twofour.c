/**
 *  $CC -Wall -ggdb3 twofour.c -o twofour
 **/
#include <stdio.h>
typedef struct twofournode
{
	char val1[6];
	char val2[6];
	char val3[6];
	struct twofournode *lessV1;
	struct twofournode *lessV2;
	struct twofournode *lessV3;
	struct twofournode *lessInf;	

}
int main (int argc, char **argv)
{


}
