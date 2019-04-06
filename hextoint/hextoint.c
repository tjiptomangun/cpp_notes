#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int nLen ,  x;
	char *buff=argv[1];
	int nCurr=0;
	int nAccum=0;
	if(argc<2)
	{
		printf("usage hextoint ????\n");
		exit(1);
	}


	nLen = strlen(argv[1]);
	for(x = 0; x < nLen; x++)
	{
	       nCurr = 0;
               if(! ((buff[x]  >= '0' && buff[x] <= '9') ||
                      (buff[x] >= 'A' && buff[x] <= 'F') ||
                      (buff[x] >= 'a' && buff[x] <= 'f') ))
                {
                        fprintf(stderr,"non hex\n");
                        exit(0);
                }
		if(buff[x] >= '0' && buff[x] <= '9')
			nCurr = buff[x] - '0';
		
		if(buff[x] >= 'A' && buff[x] <= 'F')
			nCurr= (buff[x] - 'A') + 10;

		if(buff[x] >= 'a' && buff[x] <= 'f')
			nCurr= (buff[x] - 'a') + 10;

		nAccum *= 16;
		nAccum += nCurr;
	
	}
	printf("%d\n",nAccum);
	
}
