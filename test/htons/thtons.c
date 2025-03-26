#include <stdio.h>
int main ()
{
        unsigned short  unMsgType =0x10;
        unsigned short  nuMsgType =htons(unMsgType);
        //unMsgType =htons(unMsgType);

        printf("hi  %x    %x \n",unMsgType,nuMsgType);
}
