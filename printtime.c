#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
	char *ftime;
	time_t PreviousDate = (time_t)0x50F58B10;
	time_t CurrentDate  = (time_t)0x50F6DC90;
	ftime = ctime(&PreviousDate);	
	printf("prevdate : %s",ftime);
	ftime = ctime(&CurrentDate);	
	printf("currdate : %s",ftime);
}
