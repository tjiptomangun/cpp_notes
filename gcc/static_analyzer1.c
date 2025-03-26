#include<stdio.h>
#include<stdlib.h>
typedef struct data_next{
	struct data_next *next;
	struct data_next *prev;
}DATA_NEXT, *PDATA_NEXT;
typedef struct stacko {
	PDATA_NEXT top;
}STACKO, *PSTACKO;
/*
void stacko_push(PSTACKO pstacko) {
	PDATA_NEXT pn = NULL;
	PDATA_NEXT prev = NULL;
	if ((pn = (PDATA_NEXT)calloc(1, sizeof(DATA_NEXT)))){
		if(!pstacko->top) {
			pstacko->top = pn;
		}
		else {
			pn->next = NULL;
			pn->prev = pstacko->top;
			pstacko->top->next = pn;
			pstacko->top = prev;
		}
	}
}
*/

void stacko_pop_free(PSTACKO pstacko) {
	PDATA_NEXT  pn = NULL;
	PDATA_NEXT  prev = NULL;
	if (pstacko->top) {
		pn = pstacko->top;
		prev = pstacko->top->prev;
		if (prev){
			prev->next = NULL;
		}
		pstacko->top = prev;
		pn->prev = pn->next = NULL;
		free(pn);
	}	
}
/*
void recursive_in(PSTACKO stacko, int count) {
	if (count) {
		stacko_push(stacko);
		recursive_in(stacko, count - 1);
	}
}
*/
void recursive_alloc_in(PSTACKO stacko, int count) {
	PDATA_NEXT pn = NULL;
	PDATA_NEXT prev = NULL;
	if (count) {
		pn = (PDATA_NEXT)calloc(1, sizeof(DATA_NEXT));
		if (pn){
			if(!stacko->top) {
				stacko->top = pn;
			}
			else {
				pn->next = NULL;
				pn->prev = stacko->top;
				stacko->top->next = pn;
				stacko->top = prev;
			}
			recursive_alloc_in(stacko, count - 1);
		}
	}
}

int main (int argc, char **argv) {
	STACKO stack = {0};
	int num = 100;
	recursive_alloc_in(&stack, num);
	while (num) {
		stacko_pop_free(&stack);
		num --;
	}
	
	return 0;
}
