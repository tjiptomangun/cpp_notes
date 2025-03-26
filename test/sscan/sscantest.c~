#include <stdio.h>
#include <time.h>
int main(int argc,char *argv[])
{
   FILE * pFile;
   time_t TimeOfDay;
   char sLine[100];
   char sLine1[100];
   char sToken1[100];
   char sToken2[100];
   char sToken3[100];
   char sToken4[100];
   char sToken5[100];
   pFile = fopen(argv[1],"r");
   if(!pFile)
   {
	printf ("File %s not found\n", argv[1]);
	exit (0);
   }
   TimeOfDay = time (NULL);
   printf ("Running on %s\n", ctime_r (&TimeOfDay, sLine));
   while (fscanf (pFile, " %[^\n]", sLine) != EOF)
   {
      //sscanf (sLine, "%s%*[ \n\t]%s%*[ \n\t]%s%*[ \n\t]%s", sToken1, sToken2, sToken3, sToken4);
      //sscanf (sLine, "%s%*[ \t]%s%*[ \t]%s%*[ \t]%s", sToken1, sToken2, sToken3, sToken4);
      sscanf (sLine, "%s %s %s %s", sToken1, sToken2, sToken3, sToken4);
      printf ("line  ..... %s\n",sLine);
      printf ("scanf ..... <1 = %s/> <2 = %s/> <3 = %s/> <4 = %s/>\n", sToken1, sToken2, sToken3, sToken4);
      if (strcmp (sToken3, "Receive") == 0)
      {
            printf("got Receive\n");
            continue;
      }
      if (strcmp (sToken3, "MMAlloc") == 0)
      {
            printf("got Malloc\n");
            continue;
      }
      if (strcmp (sToken3, "MMFree") == 0)
      {
            printf("got Free\n");
            continue;
      }
   }
}