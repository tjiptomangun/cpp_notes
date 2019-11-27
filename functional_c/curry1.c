//http://almostvalid.blogspot.com/2013/05/curried-functions-in-c.html

#include <stdio.h>
#include <stdlib.h>

typedef int (*fun_t)(int);

int bar(int a){
    return a * 5;
}

fun_t foo(void){
    return bar;
}

void some_fun(void){
    foo()(5);
}

#define DECLARE_CURRIED_FUNCTION(name, type, op)\
    typedef type (*_curry_##name##_t) (type); \
    type _curried_a_##name; \
    type _curried_ret_##name(type b) {\
        type a = _curried_a_##name; \
        op; \
    }\
    _curry_##name##_t name(type a) {\
        _curried_a_##name = a; \
        return _curried_ret_##name; \
    }

#define DECLARE_CURRIED_2_FUNCTION(name, ret_type, p1_type, p2_type, op)\
    typedef ret_type (*_curry_##name##_t) (p2_type); \
    p1_type _curried_a_##name; \
    ret_type _curried_ret_##name(p2_type b) {\
        p1_type a = _curried_a_##name; \
        op; \
    }\
    _curry_##name##_t name(p1_type a) {\
        _curried_a_##name = a; \
        return _curried_ret_##name; \
    }
    
    

int main (int argc, char **argv) {
    DECLARE_CURRIED_FUNCTION(add, int, return a + b);
    DECLARE_CURRIED_2_FUNCTION(add_str, int, char *, char *, return atoi(a) + atoi(b));

    int j = add(5)(3);

    printf("j = %d\n", j);

    j = add_str("11")("23");

    printf("j_str = %d\n", j);

    return 0;
}
    
