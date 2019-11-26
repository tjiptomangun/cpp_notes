#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
     fprintf(stdout ,"outss\n");
     fprintf(stderr ,"errss\n");
     while(1)
	sleep(1);
     return 0;
}
