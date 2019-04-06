#include <stdio.h>
#include <stdlib.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <unistd.h>

#define MAX_IFS 64
typedef struct lic
{
	unsigned int hostid;	
	char mac[6];
}LIC, *PLIC;
int main (int argc, char **arg)
{
	unsigned int hostid = 0; 
	struct ifreq ifs[MAX_IFS];
	struct ifconf ifc;
	struct ifreq ifreq;
	struct ifreq *ifr, *ifend;
	char if_buf[4096];
	hostid = (unsigned int) gethostid();
	fprintf (stdout, "hostid = %d\n", hostid);

	int fd = socket (PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (fd < 0)
	{
		perror ("socket");
		exit (EXIT_FAILURE);
	}

	ifc.ifc_len = sizeof (ifs);
	ifc.ifc_req = ifs;
	ifc.ifc_buf = if_buf;
	if (ioctl (fd, SIOCGIFCONF, &ifc) == -1)
	{
		perror ("ioctl.SIOGIFCONF");
		exit (EXIT_FAILURE);
	} 
	ifend = ifs + (ifc.ifc_len/sizeof (struct ifreq));;
	for (ifr = ifc.ifc_req; ifr < ifend; ++ifr)
	{
		if (ifr->ifr_addr.sa_family == AF_INET)
		{
			fprintf (stdout, "want to print\n");
			fprintf (stdout, "%s\n", ifr->ifr_name);	
			strcpy (ifreq.ifr_name, ifr->ifr_name); 
			if (ioctl (fd, SIOCGIFFLAGS, &ifreq) == 0 && 
				!(ifreq.ifr_flags & IFF_LOOPBACK))
			{
				if (0 == ioctl (fd, SIOCGIFHWADDR, &ifreq))
				{
					fprintf (stdout, "%02x:%02x:%02x:%02x:%02x:%02x::%d\n",
						ifreq.ifr_addr.sa_data[0],
						ifreq.ifr_addr.sa_data[1], ifreq.ifr_addr.sa_data[2],
						ifreq.ifr_addr.sa_data[3], ifreq.ifr_addr.sa_data[4],
						ifreq.ifr_addr.sa_data[5],hostid);
						
				}


			}
		}
	}

	
}
