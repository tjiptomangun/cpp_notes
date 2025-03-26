//https://stackoverflow.com/q/61232299
#include <stdio.h>
int a = 1000;

struct trampo02struct {
	int running;
	int result;
	struct trampo02struct* (*nextFun)(struct trampo02struct * );
};

void trampoline02(struct trampo02struct *f) {
  while (f->nextFun) {
    struct trampo02struct *g = f->nextFun(f);
	f = g;
  }
}


struct trampo02struct *f1(struct trampo02struct *in) {
  int tmp;
  if (in->running == 0) {
    in->nextFun = NULL;
	in->running = in->result;
	return in;
  } else {
    in->nextFun = f1;
	tmp = in->running;
	in->result = in->running + in->result;
	in->running --; 
	return in;
  }
}


int main() {
  
  struct trampo02struct k = {
	.nextFun = f1,
    .running = 100,
	.result = 0 
  };

  trampoline02(&k);
  fprintf(stdout, "%d\n", k.result);
  return 0;
}
