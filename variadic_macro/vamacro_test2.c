#include <stdio.h>
#include <stdarg.h>

void realdbgprintf(const char *file_name, const char *func_name, int line_no, const char *formats, ...) {
    va_list args;
    va_start (args, formats);

    fprintf (stdout, "%s : %s : %d : ", file_name, func_name, line_no);
    vfprintf(stdout, formats, args);
    fprintf (stdout, "\n");
    va_end(args); 
}

#define dbgprintf(...)realdbgprintf(__FILE__, __func__, __LINE__, __VA_ARGS__)


int test_function(int j){
    dbgprintf("in func  %d ", j);
    return j + 1;
}

int just_function(int j){
    dbgprintf("out func %d ", j+j);
    return j + 1;
}
int main (int argc, char**argv){
    test_function(3);

    test_function(4);

    just_function(5);
}
