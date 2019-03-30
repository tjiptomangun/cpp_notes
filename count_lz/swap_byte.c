#include <stdio.h>
#define uint32_t unsigned int


#define __bswap_constant_32(x) \
     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |               \
      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))


int main(int argc, char *argv[])
{
   unsigned int a = 0x00000000;
   unsigned int b = 0x80000000;
   unsigned int c = 0xffffffff;
   unsigned int d = 0x0000ffff;
   unsigned int e = 0xffff0000;
   unsigned int f = 0x08000000;
   unsigned int g = 0x00000080;
   unsigned int h = 0x1f000000;
   unsigned int i = 0x000000f1;

   printf("a = %08x ",a);a = __bswap_constant_32(a);printf("swapped= %08x\n",a);
   printf("b = %08x ",b);b = __bswap_constant_32(b);printf("swapped= %08x\n",b);
   printf("c = %08x ",c);c = __bswap_constant_32(c);printf("swapped= %08x\n",c);
   printf("d = %08x ",d);d = __bswap_constant_32(d);printf("swapped= %08x\n",d);
   printf("e = %08x ",e);e = __bswap_constant_32(e);printf("swapped= %08x\n",e);
   printf("f = %08x ",f);f = __bswap_constant_32(f);printf("swapped= %08x\n",f);
   printf("g = %08x ",g);g = __bswap_constant_32(g);printf("swapped= %08x\n",g);
   printf("h = %08x ",h);h = __bswap_constant_32(h);printf("swapped= %08x\n",h);
   printf("i = %08x ",i);i = __bswap_constant_32(i);printf("swapped= %08x\n",i);
}