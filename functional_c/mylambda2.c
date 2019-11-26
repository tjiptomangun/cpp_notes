//https://blog.noctua-software.com/c-lambda.html
#include <stdio.h>
void func(int nb, void (*f)(int)){
    int i;
    for(i = 0; i < nb; i ++) f(i);
}

int main () {
    void callback(int v){
        printf("%d\n", v);
    }

    func(4, callback);
    fprintf (stdout, "====\n");
    func(4, ({
        void _(int v){
            printf("%d\n", v);
        }
        _;
    }));
}
