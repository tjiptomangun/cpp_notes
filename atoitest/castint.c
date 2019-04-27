#include <stdio.h>

int main (int argc, char **argv) {
  char chr [] = {(char)1,(char)0,(char)0,(char)0, (char)0};
  int *int_ptr = (int*) chr;
  int int_mod = *int_ptr;
  int_mod ++;

  fprintf(stdout, "int_ptr : %d\n", *int_ptr);
  fprintf(stdout, "int_mod: %d\n", int_mod);

}
