#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

void logger(const char* format, ... ) {
  FILE *file = fopen("/var/log/smpppdu.log", "a+");
  if (file) {
	char buffer[26];
  	int millisec;
  	struct tm* tm_info;
  	struct timeval tv;

  	gettimeofday(&tv, NULL);

  	millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
  	if (millisec>=1000) { // Allow for rounding up to nearest second
    	millisec -=1000;
    	tv.tv_sec++;
  	}

  	tm_info = localtime(&tv.tv_sec);

  	strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
	fprintf (file, "%s: ", buffer);
	va_list args;
	va_start (args, format);
	vfprintf (file, format, args);
	fprintf (file, "%s", "\n");
	va_end (args);
	fclose(file);
  } else {
	  strerror(errno);
	  exit(1);
  }
}
