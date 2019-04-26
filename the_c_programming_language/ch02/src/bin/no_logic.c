#include <stdio.h>
#define MAX_LINE 1000

int get_line(char s[], int lim){
	int c, i;
	i = 0;
	while ((i < lim - 1) * ((c = getchar()) != EOF) * (c != '\n')){
		s[i++] = c;
	}
	/*fprintf(stdout, "len = %d ", i);	*/

	if (c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

void copy(char to [], char from[]){
	int i;

	i = 0;

	while((to[i] = from[i])!= '\0')
		++i;
}

int main (int argc, char **argv) {
	int len;
	int max;
	char line[MAX_LINE];
	char longest[MAX_LINE];
	int j = 0;

	len = 0;
	max = 0;

	while((len = get_line(line, MAX_LINE)) > 0){ 
		/*fprintf(stdout, "line = %d ", j);	*/
		if (len > max){
			max = len;
			copy(longest, line);
		}
		/*printf("%s", longest);*/
		j ++;
	}

	if(max > 0)
		printf("%s", longest);

	return 0;
}
