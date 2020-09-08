//https://www.tutorialcup.com/interview/linked-list/quick-sort-singly-linked-list.htm
/* Partition Algorithm 
   *1. Take rightmost element as the pivot 
       *Traverse through the list 
       * a. If the node has value greater than pivot, we will move it after tail
       * b. else, keep it in same position

   *2 QuickSort Algorithm 
      *1. Call Partition(), which places pivot at right position and returns the pivot 
      *2. Find the tail node of the left sublist ie, left side of the pivot and recur for left list 

   *3.  Now, recur for right list
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct ll_node{
	int data;
	struct ll_node *next; 
} LL_NODE, *PLL_NODE;

void prepend(PLL_NODE *phead, int inserted) {
	PLL_NODE curr = calloc (1, sizeof (LL_NODE));
	curr->data = inserted;
	curr->next = NULL;
	if (*phead) {
		curr->next = *phead;
	}
	*phead = curr;
}

void display (PLL_NODE *phead) {
	PLL_NODE temp = *phead;
	while (temp) {
		if (temp->next) {
			fprintf(stdout, "%d ->", temp->data);
		}
		else {
			fprintf(stdout, "%d", temp->data);
		}
		temp = temp->next; 
	}
	fprintf(stdout, "\n");
}

PLL_NODE get_tail (PLL_NODE curr) {
	while(curr && curr->next) {
		curr = curr->next;
	}
	return curr;
}

// Partitions the list , taking the last element as the pivot
PLL_NODE partition(PLL_NODE head, PLL_NODE end, PLL_NODE *new_head, PLL_NODE *right_tail, PLL_NODE *new_end) {
	PLL_NODE pivot = end;
	PLL_NODE prev = NULL;
	PLL_NODE curr = head;
	PLL_NODE tail = pivot;

	while (curr != pivot) {
		if (curr->data < pivot->data) {
			//First node with values less than pivot
			//becomes the new_head
			if ((*new_head == NULL)) {
				*new_head = curr;
			}
			prev = curr;
			curr = curr->next;
		}
		else {
			//curr is greater than pivot
			//move curr to end of list and change
			//tail
			//this will move all node greater than
			//pivot to the left of pivot
			if (prev) {
				prev->next = curr->next;
			}
			PLL_NODE tmp = curr->next;
			curr->next = NULL; //this will unlink tail with
					   //the rest of list
					   //later in quick_sort_recur
					   //they are linked again
			tail->next = curr;
			tail = curr;
			curr = tmp;

		}
	}

	if(prev) {
		prev->next = NULL;
	}

	*right_tail = prev;

	//means there is no data smaller than pivot
	if (!(*new_head)) {
		*new_head = pivot;
	}

	//update new_end as the last node
	*new_end = tail;

	return pivot;
}

PLL_NODE quick_sort_recur(PLL_NODE head, PLL_NODE end) {
	if(!head || head == end) {
		return head;
	}

	PLL_NODE new_head = NULL;
	PLL_NODE new_end = NULL;
	PLL_NODE right_tail = NULL;

	//this will results in new new_head because new_head always set
	//to null
	//returned pivot will always  = end
	PLL_NODE pivot = partition(head, end, &new_head, &right_tail, &new_end);

	if (new_head != pivot) {
		//new_head smaller than end/pivot
		//new_head that returned by partition used
		//as  new head
		//re sort sub list from new_head up to
		//pivot - 1 (left side)
		//sort left part of pivot now
		new_head = quick_sort_recur(new_head, right_tail);

		//tmp = get_tail(new_head);

		(get_tail(new_head)) ->next = pivot;
		//relink sorted left side with pivot
		//tmp->next = pivot;
	}

	//sort right part of pivot
	//and link he sorted list with pivot
	pivot->next = quick_sort_recur(pivot->next, new_end);

	return new_head;
}

void quick_sort(PLL_NODE *head_ref) { 
	*head_ref = quick_sort_recur(*head_ref, get_tail(*head_ref));
}


int main (int argc, char **argv) {

	PLL_NODE init = NULL;
	prepend(&init, 6);
	prepend(&init, 16);
	display(&init); 
	quick_sort(&init);
	display(&init);

	PLL_NODE first = NULL;
	prepend(&first, 16);
	prepend(&first, 6);
	display(&first); 
	quick_sort(&first);
	display(&first);
	PLL_NODE head = NULL;

	prepend(&head, 6);
	prepend(&head, 16);
	prepend(&head, 15);
	prepend(&head, 50);
	prepend(&head, 1);
	prepend(&head, 23);
	prepend(&head, 213);
	prepend(&head, -1);
	prepend(&head, -344);
	prepend(&head, 344);
	prepend(&head, 10);
	prepend(&head, 15);
	prepend(&head, 12);
	prepend(&head, 11);
	prepend(&head, 31);
	prepend(&head, 22);
	prepend(&head, 2);
	prepend(&head, 7);
	prepend(&head, 9);


	display(&head); 
	quick_sort(&head);
	display(&head);

}

