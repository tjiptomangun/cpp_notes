#include <stdio.h>
int main(int argc, char *argv[])
{
	int ret;
	ret = 0;
	while (1==1)
	{
		printf("ret = %d\n",ret);
		ret++;
		ret%=10000;
	//	usleep(1000);
	}

}
