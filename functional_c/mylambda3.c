//https://blog.noctua-software.com/c-lambda.html
//to expand macro use gcc -E filename.c

#define LAMBDA(c_) ({c_ _;})
#include <stdio.h>
void func(int nb, void (*f)(int)){
    int i;
    for(i = 0; i < nb; i ++) f(i);
}

int main () {
    func(4, LAMBDA(void _(int v){
        printf("%d\n", v);
    }));
}
