#include <getopt.h>
#include <stdio.h>
extern int unit_test();
extern int mem_test();

int main (int argc, char **argv) { 
	int c;
	int flag = 0;
	while(-1 !=(c = getopt(argc, argv, "um"))) {
		switch (c){
			case 'u':
				flag = 0;
				break;
			case 'm':
				flag = 1;
				break;
			default:
				fprintf(stderr, "usage: %s -flag\n", argv[0]);
				fprintf(stderr, "flags:\n");
				fprintf(stderr, "	 u :unit test\n");
				fprintf(stderr, "	 m :memcheck test\n"); 
				return -1;
		} 
	}

	if (flag == 0){
		unit_test();
	}
	else if(flag == 1){
		mem_test();
	}

	return 0;
}
