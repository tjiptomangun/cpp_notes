#ifndef _PARSERCLASS_TEST_H_
#define _PARSERCLASS_TEST_H_
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"


int file_size(FILE *fin);
char *read_file(char *filename);
void print_assertion(int assertion);
void test_fun(char *name, int fn);
#endif
