#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOOLEAN unsigned short
//Read pattern and string to match
//Returns the match indexes

int printRegCompErr(int reti)
{
  switch(reti)
  {
    case REG_BADBR:
      printf("Invalid use of back reference operator.\n");
      break;
    case REG_BADPAT:
      printf("Invalid use of pattern operators such as group or list.\n");
      break;
    case REG_BADRPT:
      printf(" Invalid use of repetition operators such as using '*' as the first character.\n");
      break;
    case REG_EBRACE:
      printf("Un-matched brace interval operators.\n");
      break;
    case REG_EBRACK:
      printf("Un-matched bracket list operators.\n");
      break;
    case REG_ECOLLATE:
      printf("Invalid collating element.\n");
      break;
    case REG_ECTYPE:
      printf("Unknown character class name.\n");
      break;
    case REG_EEND:
      printf("Non specific error.  This is not defined by POSIX.2.\n");
      break;
    case REG_EESCAPE:
      printf("Trailing backslash.\n");
      break;
    case REG_EPAREN:
      printf("Un-matched parenthesis group operators.\n");
      break;
    case REG_ERANGE:
      printf("Invalid  use  of  the range operator, e.g., the ending point of the range \n");
      printf("occurs prior to the starting point.\n");
      break;
    case REG_ESIZE:
      printf("Compiled regular expression requires a pattern buffer larger than 64Kb.\n");
      break;
    case REG_ESPACE:
      printf("The regex routines ran out of memory.\n");
      break;
    case REG_ESUBREG:
      printf("Invalid back reference to a subexpression.\n");
      break;

  }
  return 0;

}//printRegCompErr


/*
 * NAME         : MaskWithPattStr
 * DESCRIPTION  : Masking a string. The masked part is
 *                either those which match the pattern or those are not.
 *                The masking character is #. We assume the string will not
 *                be long,
 * Input:
 *          sPatt      : Pattern used to match string. Extended Regex
 *          bMaskMatch : If TRUE then Mask the Match part,
 *                       otherwise the unmatch part
 *                       that will be masked
 *          toMask     : string to match
 *          nLen       : the length of the string
 *          outBuff    : buffer to store the masking results
 *          outBuffLen : length of outBuff
 * Created     : 20130122
 * */

int MaskWithPattStr(char *sPatt, BOOLEAN bMaskMatch, char *toMask, int nLen, char *outBuff, int nOutBuffLen)
{
     int reti, i;
     int x=nLen;
     int y=nOutBuffLen;
     int nDx=0;
     regex_t regex_fpatt;
     const size_t max_groups = 9;
     regmatch_t pm[max_groups];
     char *buff;
     char *temp = outBuff;
     printf("sPatt =%s  toMatch = %s \n",sPatt,toMask);
     reti = regcomp(&regex_fpatt,sPatt,REG_EXTENDED);
     if(reti)
     {
              printRegCompErr(reti);
	      regfree(&regex_fpatt);
        return 1;
     }
     buff = toMask;
     reti = regexec(&regex_fpatt,buff,max_groups,pm,0);
     while (reti == 0)
     {
       for( i = 0; i < max_groups ; i++)
       {
         if(pm[i].rm_so == -1)
           break;
         if(y>0)
         {
           if(!bMaskMatch)
           {
             if(y >= pm[i].rm_so  )
             {
               memset(temp,'*',pm[i].rm_so); 
               if(y >= pm[i].rm_eo)
               {
                 memcpy(temp+pm[i].rm_so,buff+pm[i].rm_so,pm[i].rm_eo-pm[i].rm_so);
               }
               else
               {
                 memcpy(temp+pm[i].rm_so,buff+pm[i].rm_so,y-pm[i].rm_so);
               }
             }
             else
             {
               memset(temp,'*',y);
             }
           }//if (!bMaskMatch)
           else//mask the match
           {//write the unmatch
             if(y >= pm[i].rm_so)
             {
               memcpy(temp,buff,pm[i].rm_so);
               if(y >= pm[i].rm_eo)
               {
                 memset(temp+pm[i].rm_so,'*',pm[i].rm_eo-pm[i].rm_so);
               }
               else
               {
                 memset(temp+pm[i].rm_so,'*',y-pm[i].rm_so);
               }
             }
             else
             {
               memcpy(temp,buff,y);
             }
           }
         }//if (y>0)
         buff+=pm[i].rm_eo;
         nDx+=pm[i].rm_eo;
         x-=pm[i].rm_eo;
         y-=pm[i].rm_eo;
         if(y>0)
         {
           temp+=pm[i].rm_eo;
         }
       }//for
       reti = regexec(&regex_fpatt,buff,max_groups,pm,REG_NOTBOL);

     }//while
     if(x > 0 && y > 0)
     {
       if(!bMaskMatch)
       {
         if(y>=x)
         {
           memset(temp,'*',x);
         }
         else
         {
           memset(temp,'*',y);
         }

       }
       else
       {
         if(y>=x)
         {
           memcpy(temp, buff, x);
         }
         else
         {
           memcpy(temp, buff, y);
         }

       }

     }
  regfree(&regex_fpatt);
  return 0;
}


int hashWithPatt(char *sPatt, char *toMatch, int nMaskMatch, int nLen)
{
   int reti;
   int x=nLen;
   int nDx=0;
   int i;
   regex_t regex_fpatt;
   const size_t max_groups = 9;
   regmatch_t pm[max_groups];
   char *buff;
   char temp[180];
   printf("sPatt =%s  toMatch = %s \n",sPatt,toMatch);
   reti = regcomp(&regex_fpatt,sPatt,REG_EXTENDED);
   if(reti)
   {
      printRegCompErr(reti);
      regfree(&regex_fpatt);
      return 1;
   }
   buff = toMatch;
   reti = regexec(&regex_fpatt,buff,max_groups,pm,0);
   while (reti == 0)
   {
       for ( i = 0; i < max_groups ; i++) 
       {
       if(pm[i].rm_so == -1)
           break;
           memset(temp,0,100);
           if(!nMaskMatch)
           {
             memset(temp,'*',pm[i].rm_so);
             memcpy(temp+pm[i].rm_so,buff+pm[i].rm_so,pm[i].rm_eo-pm[i].rm_so);
           }
           else
           {
             memcpy(temp,buff,pm[i].rm_so);
             memset(temp+pm[i].rm_so,'*',pm[i].rm_eo-pm[i].rm_so);
           }
           printf("%s",temp);
           buff+=pm[i].rm_eo;
           nDx+=pm[i].rm_eo;
           x-=pm[i].rm_eo;
       }
       reti = regexec(&regex_fpatt,buff,max_groups,pm,REG_NOTBOL);
       //reti = regexec(&regex_fpatt,buff,max_groups,pm,0);

   }
   if(x > 0)
   {
   
       if(!nMaskMatch)
       {
          memset(temp,0,100);
          memset(temp,'*',x);
          printf("%s",temp);
       }
       else
       {
          printf("%s",buff);
       }
   }
   printf("\n");
   regfree(&regex_fpatt);
   return 0;

}
int main (int argc, char **argv)
{
   char *patt;
   int ch, i =0;
   int nMaskMatch=0;
   char szOutBuff[180];
   memset(szOutBuff,0,180);
   if(argc < 3)
   {
      printf("usage: %s pattern string_to_match\n",argv[0]);
      exit(0);
   }
   if(argc > 3)
   {
      if(argv[3][0]=='U' || argv[3][0]=='u')
            nMaskMatch=1;
   }
   patt = argv[1];

   //if(hashWithPatt(patt, argv[2],nMaskMatch, strlen(argv[2]))
   while(1)
   {
   	ch = getchar();
	if(ch == (int) 'q') break;
   	if( MaskWithPattStr (patt, nMaskMatch, argv[2], strlen(argv[2]), szOutBuff, 180))
   	{
      		printf("Cannot Compile Pattern\n");
   	}
   	printf("%010i %s\n[q to exit]",i, szOutBuff);
	i++;
   }
   return 0;
}
