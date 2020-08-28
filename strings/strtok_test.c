#include <stdio.h>
#include <string.h>

int main (int argc, char **argv) {
	char *terong = "Hello, 1, 2, 4, , 5,  ";
	char tmp [40] = {0};
	strncpy(tmp, terong, 40);

	char *token_list[10] = {0};
	char *token = 0;
	int i = 0;

	token = strtok(tmp, ",");
      
	while(token && i < 10) {
		token_list[i] = token;
		token = strtok(NULL, ",");
		while (*token_list[i] == ' ' && *token_list[i]  != 0) {
			token_list[i] = token_list[i] + 1;
		}
		fprintf(stdout, "token_list[%d] length = %ld string = %s\n", i, 
			strlen(token_list[i]), token_list[i]);
	    i++;
   }

	
	
}
