#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif
#include <stdio.h>
int main(int argc, char**argv)
{
  //char* x[] = {"/bin/cat", "/etc/shadow"};
  //system("/bin/cat /etc/shadow");
   time_t   new_time;
   int result;
   struct tm      lassie;
   struct timeval garfield;
   int jsecs=0,jminutes =0,jhour=0,jdate=1,jmonth=1,jyear=2011,jmilisecs=10;
   lassie.tm_sec =(int)jsecs;
   lassie.tm_min =(int)jminutes;
   lassie.tm_hour=(int)jhour;
   lassie.tm_mday=(int)jdate;
   lassie.tm_mon =(int)jmonth-1;
   lassie.tm_year=(int)jyear-1900;
   new_time = mktime(&lassie);
   garfield.tv_sec = new_time;
   garfield.tv_usec = (int)jmilisecs;

   result = settimeofday(&garfield,NULL);
   if(result < 0)
   {
      saved_error = errno;

      if(saved_error == EPERM)
      {
         fprintf(stderr,"You must run this as root/sysadmin");
         return 1;
      }
   }
   return 0;
}
