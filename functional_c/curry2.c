#include <stdio.h>

void closure_0() {
	typedef int (*int_to_int) (int);
	int_to_int fun1 (int a){
		int fun1_var = a;
		fprintf(stdout, "fun1_var : %d\n", fun1_var);
		int fun2 (int in) {
			fprintf(stdout, "fun1_var : %d\n", fun1_var); 
			return in * 1 + (fun1_var * 10);
		}
		return fun2;
	}
	fprintf(stdout, "closure_0 fun1(3)(2) = %d\n", fun1(3)(2));
}

void closure_1() {
	typedef int (*int_to_int) (int);
	typedef int_to_int (*int_to__int_to_int) (int);
	int_to__int_to_int fun1(int a) {
		int fun1_var = a;
		fprintf(stdout, "fun1_var : %d\n", fun1_var);
		int_to_int fun2(int b) {
			fprintf(stdout, "fun1_var : %d\n", fun1_var);
			int fun2_var = b;
			int fun3(int c) {
				fprintf(stdout, "fun1_var : %d\n", fun1_var); 
				return c * 1 + fun2_var * 10 + fun1_var * 100;
			}
			return fun3;
		}
		return fun2;
	} 
	fprintf(stdout, "closure_1 fun1(3)(2)(1) = %d\n", fun1(3)(2)(1));
}

int main (int argc, char **argv) {
	closure_0();
	closure_1(); 
}
