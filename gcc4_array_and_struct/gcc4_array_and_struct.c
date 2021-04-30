//gcc -Wall -ggdb3  gcc4_array_and_struct.c -o gcc4_array_and_struct
#include <stdio.h>

struct test_struct {
	int id;
	char name[20];
	char *ptr_next;
};

int test_fun () { 
	int i = 0;
	struct test_struct test_struct_d[20] = {{0}}; 
	fprintf(stdout, "pre[0] [19] %s %s\n", test_struct_d[0].name, test_struct_d[19].name);
	fprintf(stdout, "pre[0] [19] %d %d\n", test_struct_d[0].id, test_struct_d[19].id);
	for (i = 0; i < 20; i++) {
		sprintf(test_struct_d[i].name, "test %02d", i);
		test_struct_d[i].id = i;
	}
	fprintf(stdout, "pos [0] [19] %s %s\n", test_struct_d[0].name, test_struct_d[19].name);
	fprintf(stdout, "pos [0] [19] %d %d\n", test_struct_d[0].id, test_struct_d[19].id);
	return 1;
}
int main (int argc, char **argv) {
	int i;
	for(i = 0; i < 10; i++ ) {
		test_fun();
	}
	return 0;
}
