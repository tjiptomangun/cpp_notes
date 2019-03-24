#include "myxim.h" 
int init_shm (unsigned char **dest, char *shm_name, int shm_size)
{
	int fd;
	char temp[80];

	if (( fd = shm_open (shm_name, O_RDWR, 0777)) == -1)
	{
		if (errno != ENOENT)
		{
			fprintf (stderr, "\n%d %d shm_open new: %s", getpid(), 
				errno, shm_name);
			perror (temp);
			exit (EXIT_FAILURE);
		}
		if ((fd = shm_open (shm_name, O_RDWR|O_CREAT, 0777)) == -1)
		{
			sprintf (temp, "\n%d %d shm_open fails: %s", getpid(), 
				errno, shm_name);
			perror (temp);
			exit (-1);
		}
		if (ftruncate (fd, shm_size) == -1)
		{
			sprintf (temp, "\n%d %d ltrunc fails: %s %d", getpid(), 
				errno, shm_name, shm_size);
			perror (temp);
			exit (-1); 
		}
	}
	if ((*dest = mmap (0, shm_size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		sprintf (temp, "\n%d %d mmap fails: %s size %d", 
			getpid(), errno, shm_name, shm_size);
		perror (temp);
		shm_unlink (shm_name);
			exit (-1); 
	}
	
	close (fd);
	return 0; 
}


int initblock ()
{
	int ret;
	ret = init_shm ((unsigned char **) &pIpcPoolBitmap, "/ipcmap", maxShm>>8);
	ret = init_shm ((unsigned char **)&shm, 
			"/shmblock", maxShm* (sizeof (MYXIM) -1 + MAX_DATA));
	return ret; 
}


int deinitblock ()
{
	return 0;
}


