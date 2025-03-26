#include <unistd.h>
void main()
{
	int ret;
	ret = execl("/bin/vicks","vicks","/home/henky/Documents/bmsmatchlog2.sql",NULL);
	if(ret == -1)
		perror("execl");
	
}