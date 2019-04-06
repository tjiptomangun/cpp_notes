
#include <regex.h>
#include <stdio.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    regex_t regex;
    int reti;
    char msgbuf[100];
    //reti = regcomp(&regex,"^a[[:alnum:]]",0);
    char * filedate = argv[1];
    reti = regcomp(&regex,"^%s",argv[2]);
    if(reti){fprintf(stderr,"could not compile regex\n"); exit(1);}
    reti = regexec(&regex,"abc",0,NULL,0);
    if(!reti)
    {
       puts("Match");
    }
    else if(reti == REG_NOMATCH)
    {
       puts("No Match");
    }
    else
    {
      regerror(reti,&regex,msgbuf,sizeof(msgbuf));
      fprintf(stderr,"Regex match failed: %s\n",msgbuf);
      exit(1);
    }

    regfree(&regex);
    return 0;

}
