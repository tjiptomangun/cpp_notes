//https://beginnersbook.com/2015/02/quicksort-program-in-c/
//
#include <stdio.h>
void quicksort(int number[], int first, int last) {
	int i, j, pivot, temp;
	if (first < last){
		pivot = first;
		i = first;
		j = last;

		while (i < j) {
			while (number[i] <= number[pivot] && i < last) {
				i++;
			}
			while (number[j] > number[pivot]){
				j--;
			}

			//there are right_element in left_hand
			//and left_element in right_hand
			//exchange
			if (i < j) {
				temp = number[i];
				number[i] = number[j];
				number[j] = temp;
			}
		}//repeat

		//i and j is overlapped
		temp = number[pivot];
		number[pivot] = number[j];
		number[j] = temp;
		quicksort(number, first, j - 1);
		quicksort(number, j + 1, last);
	}
}

int main (int argc, char **argv) {
	int i, count, number[25] = {0};

	fprintf(stdout, "enter number of elements\n");
	scanf("%d", &count);
	fprintf(stdout, "enter %d integers\n", count);

	for (i = 0; i < count; i ++) {
		scanf("%d", &number[i]);
	}

	quicksort(number, 0, count - 1);

	for (i = 0; i < count; i++) {
		if (i > 0){
			fprintf(stdout, " ");
		}
		fprintf(stdout, "%d", number[i]);
	}

	fprintf(stdout, "\n");
}
