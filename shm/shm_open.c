/*
  http://menehune.opt.wfu.edu/Kokua/More_SGI/007-2478-008/sgi_html/ch03.html
  Program to test shm_open(3)
  shm_open [-p <perms>] [-s <bytes>] [-c] [-x] [-r] [-t] [-w] <path>
  - p <perms> access mode to use when creating, default 0600
  - s <bytes> size of segment to map, default 64k
  - c use O_CREAT
  - x use O_EXCL
  - r use O_RDONLY, default is O_RWR
  - t use O_TRUNC
  - w wait for keyboard input before exiting
  <path> the pathname of the queue , required
*/

#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char ** argv)
{

  int perms = 0600;  /*permissions*/
  size_t size = 65536; /*segment size*/
  int oflags = 0;
  int ropt = 0;
  int wopt = 0;
  int shm_fd;
  int mprot= PROT_READ;
  int mflags = MAP_SHARED;
  void ** attach;
  char *path;
  int c;

  while(-1 !=(c = getopt(argc, argv, "p:s:cxrtw")))
  {
     switch(c)
     {
         case 'p' :/*permissions*/
           perms = (int) strtoul(optarg,NULL,0);
           break;
         case 's' : /*segment size*/
           size = (size_t) strtoul(optarg,NULL,0);
           break;
         case 'c' : /*use O_CREAT*/
           oflags |= O_CREAT;
           break;
         case 'x' : /*use O_EXCL*/
           oflags |= O_EXCL;
           break;
         case 't' : /*use O_TRUNC*/
           oflags |= O_TRUNC;
           break;
         case 'r' : /*use O_RDONLY*/
           ropt = 1;
           break;
         case 'w' :/*wait after attaching*/
           wopt = 1;
           break;
         default :/*unknown or missing argument*/
           return -1;
     }/*switch*/
  }/*while*/
  if(optind < argc)
    path = argv[optind++]; /*first no option argument*/
  else
    {
      printf("Segment pathname required\n");
      return -1;
    }
  if(0 == ropt)
  {/*read-write access, reflect in mprot and mflags*/
    oflags |=O_RDWR;
    mprot |= PROT_WRITE;
    mflags |=MAP_AUTOGROW + MAP_AUTORESRV;
  }
  else
  {
    oflags!=O_RDONLY;
  }

  shm_fd = shm_open(path,oflags,perms);
  if(-1 != shm_fd)
  {
     
  }
}
