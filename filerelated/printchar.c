#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main (int argc, char **argv)
{ 
	int i = 0;
	char arr_buff[512];
	ssize_t readbyte; 
	int in = open ("fw_cut3.csv",O_RDONLY);
	int out = open ("fw_cut4.csv",O_CREAT|O_WRONLY, 0666);

	memset (arr_buff, 0, 512);
	if (in < 0)
	{
		perror ("open");
		exit (-1);
	} 
	if (out < 0)
	{
		perror ("open");
		exit (-1);
	}
	readbyte = read (in, arr_buff, 512);
	printf ("numbytes = %i", readbyte);
	for (i = 0; i < readbyte; i++)
	{
		if (i%10 == 0)
			printf ("\n");
		if (arr_buff[i] == 0x0a)
			arr_buff[i] = 0x0d;
		write (out, &arr_buff[i], 1);
		printf ("%02x ", arr_buff[i]);
	}
	close (in);
	close (out);
	exit (EXIT_SUCCESS);
	
}
