#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct have_next{
	char data [20];
	struct have_next * next; 
} HAVE_NEXT, *PHAVE_NEXT;

PHAVE_NEXT new_have_next(char *name) {
	PHAVE_NEXT ret = calloc(1, sizeof(HAVE_NEXT));
	if (ret) {
		strncpy(ret->data, name, 20);
	}
	return ret;
}

PHAVE_NEXT new_have_prev(char *name, PHAVE_NEXT prev) {
	PHAVE_NEXT ret = NULL;
	if ((ret = new_have_next(name))) {
		prev->next = ret;
	}
	return ret;
} 

void print_list(PHAVE_NEXT head) {
	PHAVE_NEXT curr = head;
	while(curr) {
		fprintf(stdout, "%s ", curr->data);
		curr = curr->next; 
	}
	fprintf(stdout, "\n");
}
int main (int argc, char **argv) {
	PHAVE_NEXT head ;
	PHAVE_NEXT curr = new_have_next("cx");
	head = curr;
	curr = new_have_prev("ax", curr);
	curr = new_have_prev("zx", curr);
	curr = new_have_prev("qx", curr);
	curr = new_have_prev("tx", curr);
	curr = new_have_prev("fx", curr);
	curr = new_have_prev("dx", curr);
	curr = new_have_prev("ex", curr);
	curr = new_have_prev("ay", curr); 
	curr = new_have_prev("ry", curr); 
	curr = new_have_prev("ty", curr); 
	curr = new_have_prev("kx", curr); 


	fprintf(stdout, "initial position : \n");
	print_list(head);

	PHAVE_NEXT start = head;
	PHAVE_NEXT pos;
	while(start != NULL) {
		pos = start->next;
		while (pos) { 
			if (strcmp(pos->data, start->data) < 0) {
				char data[20] = {0};
				strcpy(data, start->data);
				strcpy(start->data,pos->data);
				strcpy(pos->data, data);
			} 
			pos = pos->next;
		}
		start = start->next; 
	}
	fprintf(stdout, "final position : \n");
	print_list(head);

	return 0;
}
