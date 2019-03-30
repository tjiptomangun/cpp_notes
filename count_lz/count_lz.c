#include <stdio.h>
#define NumLeadingZeros(x) (0 == (x)) ? 32 : __builtin_clz(x); 
int main(int argc, char *argv[])
{
   unsigned long k = 0xf0ffff00;
   unsigned long l = 0xe0ffff00;
   unsigned long m = 0xe0ffff00;
   unsigned long n = 0x00ffff00;
   unsigned long o = 0xffffffff;
   unsigned long p = 0x00000000;
   int nPos=0;
   unsigned long a  = ~n;
   printf("n=%x  o=%x\n",n,a);
   nPos = NumLeadingZeros(n);
   printf("num leading zero 0 of %08x=%d\n",n,nPos );
   nPos = NumLeadingZeros(a);
   printf("num leading zero 0 of %08x=%d\n",a,nPos );

   a  = ~o;
   printf("o=%x  a=%x\n",o,a);
   nPos = NumLeadingZeros(o);
   printf("num leading zero 0 of %08x=%d\n",o,nPos );
   nPos = NumLeadingZeros(a);
   printf("num leading zero 0 of %08x=%d\n",a,nPos );

   a  = ~p;
   printf("p=%x  a=%x\n",p,a);
   nPos = NumLeadingZeros(p);
   printf("num leading zero 0 of %08x=%d\n",p,nPos );
   nPos = NumLeadingZeros(a);
   printf("num leading zero 0 of %08x=%d\n",a,nPos );
}