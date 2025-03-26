#include "stdio.h"


int print_non_static()
{
	int j=0;
	j++;
	printf("non static j = %d\n", j);
}
int print_static()
{

	static int j=0;
	j++;
	printf("static j = %d\n", j);

}

int main(int argc, char **argv)
{
	int k;
	for (k = 0; k < 10; k++)
	{
		print_non_static();
		print_static();
	}
}
