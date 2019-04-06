#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/cdrom.h>

#define CDDEVICE "/dev/cdrom"
int main(int argc, char **argv)
{
  int cdrom;
  if( (cdrom = open(CDDEVICE,O_RDONLY|O_NONBLOCK)) <0)
  {
      perror("open");
      exit(1);
  } 

  if(ioctl(cdrom,CDROMEJECT,0)<0)
  {
     perror("ioctl");
     exit(1);
  }
  close(cdrom);
}
