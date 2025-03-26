#include <stdio.h>
const char *one = "abc";
const char *two= "def";

void callcharptr(char *ptr1, char *ptr2) {
	ptr1 = one;
	ptr2 = two; 
}
void callcharptrptr(char **ptr1, char **ptr2) {
	*ptr1 = one;
	*ptr2 = two; 
}

int main (int argc, char **argv) {
	char *x1 = NULL;	
	char *x2 = NULL;	
	char *y1 = NULL;	
	char *y2 = NULL;	

	callcharptr(x1, x2);
	fprintf(stdout, "x1:%p x2:%p\n", x1, x2);
	callcharptrptr(&y1, &y2);
	fprintf(stdout, "y1:%p y2:%p\n", y1, y2);
}
