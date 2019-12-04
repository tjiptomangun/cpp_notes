#include <stdio.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

int main (int argc, char **argv) {
    eprintf("%s: %d\n", argv[1], argc);    
}
