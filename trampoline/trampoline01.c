//https://stackoverflow.com/q/61232299
#include <stdio.h>
int a = 1000;

void trampoline(void *(*f)()) {
  while (f) {
    void *g = f();
    f = (void *(*)())g;
  }
}

void *f1();
void *f2();

void *f1() {
  if (a == 0) {
    return NULL;
  } else {
    return f2;
  }
}

void *f2() {
  --a;
  return f1;
}

int main() {

  trampoline(f1);
  return 0;
}
