#include<stdio.h>
#include<stdlib.h>

int main (int argc, char *arg[])
{
	char buff[BUFSIZ];
	ssize_t	n;
	int x;
	n = fread(buff,1,sizeof(buff),stdin);
	for(x=0;x<n; x++)
	{
		if(! ((buff[x]>='0' && buff[x]<='9') ||
                      (buff[x]>='A' && buff[x]<='F') ||
                      (buff[x]>='a' && buff[x]<='f') ))
		{
			fprintf(stderr,"non hex");
			exit(0);
		}
	}

}
