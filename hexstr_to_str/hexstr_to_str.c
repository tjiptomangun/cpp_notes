#include <stdio.h>
#include <string.h>

int hex_to_int(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

int hex_to_ascii(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}

int main(int argc , char * argv[]){
        //const char* st = "48656C6C6F3B";
        if(argc<2)
	{
	       printf("usage : %s string_to_convert",argv[0]);
	}
        const char* st = argv[1];
        int length = strlen(st);
        int i;
        char buf = 0;
        for(i = 0; i < length; i++){
                if(i % 2 != 0){
                        printf("%c", hex_to_ascii(buf, st[i]));
                }else{
                        buf = st[i];
                }
        }
        printf("\n");
}
