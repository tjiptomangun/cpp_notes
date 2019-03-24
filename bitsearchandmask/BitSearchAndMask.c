


#include<stdio.h>
void BitSearchAndMask(unsigned char bByte)
{
   unsigned char        mask = 0x80;
   int                  i;
   for (i = 0; i < 8; ++i)
   {
      if (bByte & mask)
      {
         mask >>= 1;
      }
      else
      {
         printf("{");
         printf("%d,",i);
         printf("0x%x",mask);
         printf("},");
         if(((bByte+1)%8)==0)
         {
           printf("\n");
         }
         return;
      }
   }
}
int main()
{
   unsigned  x;
   for(x=0;x<256;x++)
   {
       BitSearchAndMask((unsigned char)x);
   }
         printf("{");
         printf("%d,",-1);
         printf("0x%x",0);
         printf("}\n");
}