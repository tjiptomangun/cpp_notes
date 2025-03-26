#include "parserclass_test.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int file_size(FILE *fin) {
  int nPos,nFSize;
  nPos=ftell(fin);
  fseek(fin,0L,SEEK_END);
  nFSize=ftell(fin);
  fseek(fin,nPos,SEEK_SET);
  return nFSize;
}
char * read_file(char *filename) {
	ssize_t n_read = 0;
	ssize_t n_buff = 0;
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	else {
		int fsz = file_size(fp);
		char * holder = calloc(fsz + 1, sizeof(char));
		char *ptrh = holder;
		do {
				ptrh+=n_read;
				n_read = fread(ptrh, 1, 32, fp);
				n_buff += n_read;
			//fprintf(stdout, "n_read = %zd n_buff = %zd\n", n_read, n_buff);
		}while(n_read == 32 && n_buff < fsz);
		fclose(fp);
		return holder;
	}
}

void print_assertion(int assertion) {
  if (assertion){
    printf("\n%sOk", KBLU);
  }
  else{
    printf("\n%sfailed", KRED);
  }
  printf("%s\n", KNRM);
}
void test_fun(char *name, int fn){
	fprintf(stdout, "%s+%s\n", KCYN, name); 
	printf("%s", KNRM);  
	print_assertion(fn);
}
