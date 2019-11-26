//https://www.codeproject.com/tips/614570/lambda-in-c-gcc
#include <stdio.h>

int test(void (*ff1)()){
    ff1();

    ({void _() {
        printf("Mars!\n");
    } (void(*)()) _;
    })();

    return 0;
}

int main (int argc, char **argv) {
    void (*MySub2) () ;

    /*_ is just function name, can be anything*/
    void (*MySub1) () = ({void _() {
        printf ("Hello ");
    } _; 

    /*can also be written with explicit type as below*/

    /*} (void (*)()) _;*/
    });

    MySub2 = MySub1;

    MySub2();
    /*create and call lambda*/
    ({void _() {
        printf("World!\n");
    } (void(*)()) _;
    
    })();

    test(MySub1);

    return 0;

}
