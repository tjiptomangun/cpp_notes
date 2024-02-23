#ifndef _MISC_FUNC_
#define _MISC_FUNC_
unsigned int fib(int nthx) {

    unsigned int innerfib(unsigned int nth, unsigned int vn_1, unsigned int vn) {
        if (nth == 0){
            return vn_1;
        }
        else if (nth == 1){
            return vn;
        }
        else {
            return innerfib(nth - 1, vn, vn_1 + vn);
        }
    }
    return innerfib(nthx, 0, 1);
}
#endif
