#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
   unsigned int theBytes ;
   unsigned int numLeadingOne;
   unsigned int _marker = 0x80000000;
   numLeadingOne = 10;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
   numLeadingOne = 0;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
   numLeadingOne = 11;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
   numLeadingOne = 32;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
   numLeadingOne = 31;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
   numLeadingOne = 33;
   theBytes = _marker>>numLeadingOne;
   printf ("numLeadingOne = %d, theBytes = %08x\n",numLeadingOne,theBytes );
}