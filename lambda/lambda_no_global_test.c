/**
 * gcc -o clambda2 --std=gnu99 clambda2.c
 * https://hackaday.com/2019/09/11/lambdas-for-c-sort-of/
 * https://github.com/wd5gnr/clambda/blob/master/clambda2.c
 */

#include <stdio.h>


#define lambda(lambda$_ret, lambda$_args, lambda$_body)\
({\
 lambda$_ret lambda$_anon$ lambda$_args\
 lambda$_body\
 &lambda$_anon$;\
})

float average_apply(float (*fn)(float inp), float thelist[], int n)
{
    int i;
    float avg=0.0;
    for (i=0;i<n;i++)
    {
        avg+=fn(thelist[i]);
        printf("Running sum=%f\n", avg);
    }

    return avg/n;
}

int main (int argc, char **argv) {
  float thelist[]={ 3.141, 6.02, 42.0, .7 };
  int n=sizeof(thelist)/sizeof(thelist[0]);
  fprintf(stdout, "%f\n", average_apply(lambda(float, (float x), { 
    float y = x/2.0;
    return 2.0 * x * x/y; 
  }), thelist, n));

  return 0;
}
