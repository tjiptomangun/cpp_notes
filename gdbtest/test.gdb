set pagination off
set logging file gdb.txt
set logging on
file helloworld
b test
r
c
quit 
