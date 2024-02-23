#include <stdio.h>
int fibTail(int n, unsigned long long a, unsigned long long b) {
	if (n == 0){
		return a;
	}
	if (n == 1) {
		return b;
	}
	return fibTail(n - 1, b, a + b);
}
int main(int argc, char **argv) {
	unsigned long long k = fibTail(200, 1, 0);
	printf("%llu\n", k)	;
	
}
