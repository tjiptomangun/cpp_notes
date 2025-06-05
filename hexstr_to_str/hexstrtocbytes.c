#include <stdio.h>
#include <string.h>

int hex_to_int(char c){
		if (c>=97) {
			c-=32;
		}
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

unsigned char hex_to_uchar(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return (unsigned char) high+low;
}

int main(int argc , char * argv[]){
        //const char* st = "48656C6C6F3B";
        if(argc<2)
		{
	       printf("usage : %s string_to_convert\n",argv[0]);
			exit(1);
		}
        const char* st = argv[1];
        int length = strlen(st);
		if (!length || length %2) {
			fprintf(stderr, "invalid input\n");
			exit(1);
		}
		unsigned char *res = (unsigned char *) calloc(length/2, sizeof(char));
        int i;
        char buf = 0;
		int j = 0;
        for(i = 0; i < length; i++){
                if(i % 2 != 0){
                        res[j++] =  hex_to_uchar(buf, st[i]);
                }else{
                        buf = st[i];
                }
        }
		for(int i = 0; i < length/2; i++) {
			printf("0x%02x, ", res[i] & 0xff);
		}
		printf("\nbytes = %d \n", length/2);
		// FILE *fp = fopen("result.bin", "wb");
		// fwrite(res, length/2, 1, fp);
		// fclose(fp); 
	
}
