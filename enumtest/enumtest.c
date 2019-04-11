#include <stdio.h>

typedef enum entest {
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE
}ENTEST;

int main (int argc, char **argv) {
	int i = 3;

	switch (i) {
		case ZERO:
			printf("ZERO\n");
			break;
		case ONE:
			printf("ONE\n");
			break;
		case TWO:
			printf("TWO\n");
			break;
		case THREE:
			printf("THREE\n");
			break;
		case FOUR:
			printf("FOUR\n");
			break;
		case FIVE:
			printf("FIVE\n");
			break;
	}
}
