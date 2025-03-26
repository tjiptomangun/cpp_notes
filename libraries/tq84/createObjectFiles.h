gcc -c       src/main.c        -o bin/main.o -m64

#
# Create the object files for the static library (without -fPIC)
#
gcc -c       src/tq84/add.c    -o bin/static/add.o  -m64
gcc -c       src/tq84/answer.c -o bin/static/answer.o -m64

#
# object files for shared libraries need to be compiled as position independent
# code (-fPIC) because they are mapped to any position in the address space.
#
gcc -c -fPIC src/tq84/add.c    -o bin/shared/add.o -m64
gcc -c -fPIC src/tq84/answer.c -o bin/shared/answer.o -m64
