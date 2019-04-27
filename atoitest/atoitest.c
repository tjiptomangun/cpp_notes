#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
  char thestring [] = {'5','1', 0};
  char thestr [] = {0};
  int theint;

  theint = atoi(thestring); 
  fprintf(stdout, "the string: %d\n", theint);
  theint = atoi(thestr);
  fprintf(stdout, "the str: %d\n", theint);
  
}
