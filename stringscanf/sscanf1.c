#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int getCMDParam(char *out, char *in, char *key) {
    char temp[500];
    int len;
    char *ptr;
    *out = 0x00;
    sprintf(temp, "%s=", key);
    len = strlen(temp);
    ptr = strstr(in, temp);
    if (ptr == NULL) return (-1);
    sscanf(ptr + len, "%500[^:\r\n]", out);
    return (strlen(out));
}

int main (int argc, char **argv) {
	char out[2048] = {0};
	char *in1  = "ID=0:ACK=QUERYSUB:RESULT=0|methodVersion=6|resultInfo=66828479577,662841770688506,1,1,,0,,0,6,,5,awn|accountingInfo=741,HWPCF132,66818110607,878,,,,|toggledAccountingInfo=,0,,HWPCF132,66818110607,878,,,,|prefixAccountingInfo=0,,,,,,,,|6-0" ;
	int k = strlen(in1);
	getCMDParam(out, in1, "ID");
	printf("inlength %d out is %s\n", k, out);
	exit(0);
}
