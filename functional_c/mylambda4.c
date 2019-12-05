#include <stdio.h>

#define lambda(type, fnparm, ...) \
({\
  type __fn__ fnparm \
    __VA_ARGS__  \
  &__fn__;\
})

int main (int argc, char **argv) {
  lambda(void, (int a, int b), {
    fprintf(stdout, "a + b  = %d\n", a+ b); 
  })(1, 2);

/*
  void (*c) (int, int) = lambda(void, (int a, int b){
    fprintf(stdout, "a + b  = %d\n", a+ b); 
  }); 
  c(1, 3);
*/

}
