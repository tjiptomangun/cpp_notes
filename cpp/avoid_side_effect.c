#include <stdio.h>

#define min_eval_twice(X, Y)  ((X) < (Y) ? (X) : (Y))

#define min_eval_once(X, Y)\
    ({ typeof (X) x_ = (X);\
       typeof (Y) y_ = (Y);\
       (x_ < y_) ? x_ : y_; })

int inc_by_two(int *a) {
    *a += 2;
     return *a;
}

int main (int argc, char **argv) {
    int a = 0;
    int b = 3;

    fprintf(stdout, "min_eval_twice : %d ",min_eval_twice(inc_by_two(&a), b));
    fprintf(stdout, "a: %d, b:%d\n", a, b);
    a = 0;
    fprintf(stdout, "min_eval_once: %d ",min_eval_once(inc_by_two(&a), b));
    fprintf(stdout, "a: %d, b:%d\n", a, b);
}
