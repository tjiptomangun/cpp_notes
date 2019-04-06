#include <stdio.h>

#define MAX_CHAR 1000

int main(int argc, char **argv){

	int c;
	int dirty_start;
	int i;
	int length;

	int line[MAX_CHAR];

	do {
		dirty_start = 0; 
		i = 0;
		while((c = getchar()) != EOF  && (c != '\n')){
			if (c != ' ' && c!= '\t'){
				if (i < MAX_CHAR - 1)
					line[i++] = c;
				dirty_start = 1;
			}
			else if (dirty_start){
				if (i < MAX_CHAR - 1)
					line[i++] = c;
			}
		}
		line[i--] = '\0';
	
		if (i > 0){
			while(i > 0 && (line[i] == ' ' || line[i] == '\t'))
				line[i--] = 0;
		}
	
		length = i + 1;
		for (i = 0; i < length; i++){
			putchar(line[i]);
		}
		if (length > 0)
			putchar('\n');
	}while(c != EOF);
	
}
