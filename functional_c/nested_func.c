#include <stdio.h>

int fun_numero_uno(int k){
	int uno_var = k;
	int fun_numero_duo(int l) {
		int duo_var = l;
		int fun_numero_trio(int m) {
			fprintf(stdout, "uno_var = %d duo_var = %d trio_var = %d\n", uno_var, duo_var, m);
			return 0;
		}
		return fun_numero_trio(3 * duo_var + 2);
	}
	return fun_numero_duo(2 * uno_var + 1); 
}

int main (int argc, char **argv) {
	fun_numero_uno(100);
	return 0;
}
