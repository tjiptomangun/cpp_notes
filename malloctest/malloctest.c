#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct test_struct
{
    char x[1000];
    char *extra_data;
}TEST_STRUCT,*PTEST_STRUCT;
int nCount = -1;
int main(int argc, char **argv)
{
   PTEST_STRUCT mal1; 
   if(argc>=2)
     nCount = atoi(argv[1]);
   while(nCount)
   {
       mal1 = (TEST_STRUCT *) malloc(sizeof(TEST_STRUCT));
       printf("mal1 %u\n", (unsigned int)mal1);
       if(!mal1)       
       {
           //sleep(1);
       }
       mal1->extra_data = (char *) malloc(1000);
       printf("extra %u\n", (unsigned int)mal1->extra_data);
       if(!mal1->extra_data)       
       {
           //sleep(1);
       }
       nCount --;

   }
   while(1)
   {
       sleep(1);
   }
   return 0;
}
