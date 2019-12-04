/**
 * gcc -o clambda2 --std=gnu99 clambda2.c
 * https://hackaday.com/2019/09/11/lambdas-for-c-sort-of/
 * https://github.com/wd5gnr/clambda/blob/master/clambda2.c
 */

#include <stdio.h>

float thelist[]={ 3.141, 6.02, 42.0, 0.7 };

#define lambda(lambda$_ret, lambda$_args, lambda$_body)\
({\
 lambda$_ret lambda$_anon$ lambda$_args\
 lambda$_body\
 &lambda$_anon$;\
})

float average_apply(float (*fn)(float inp))
{
    int i,n=sizeof(thelist)/sizeof(thelist[0]);
    float avg=0.0;
    for (i=0;i<n;i++)
    {
        avg+=fn(thelist[i]);
        printf("Running sum=%f\n", avg);
    }

    return avg/n;
}

int main (int argc, char **argv) {
  fprintf(stdout, "%f\n", average_apply(lambda(float, (float x), { 
    int y = x/2;
    return 2 * x * x/y; 
  })));
//  fprintf(stdout, "%f\n", average_apply(lambda(float, (float x), { return x / 3.0; })));

  return 0;
}
