#!/bin/bash 
./parserclass_test -c "a"\
  -l "b" -t -f "a" -l "b" -t -f "a" -l "b" -t -p\
  -e "delete all b{"\
  -t -f "a" -x "b" -t -p\
  -e "}"\ 
#  -t -p\
#  -e "add another b"\
#  -t  -f "a"\
#  -l "b"\
#  -t -p\
#  -t -f "a"\
#  -l "b"\
#  -t -p \
#  -e "remove all b"\
#  -x "b"\
#  -t -p -d
#  -t -f "a"\
#  -l "b"\
#  -t -f "a"\
#  -l "b"\
#  -t\
#  -p\
#  -f "a"\
#  -x "b"\
#  -t\
#  -p\
#  -d 
#	-t  -f  "a/b"  \
#	-r "gray"  \
#	-t \
#	-e "in b, gray removed" \
#	-p \
#	-f  "a/b"  \
#  -c "x/c" \
#  -e "in b, add element x/c" \
#  -t \
#  -p \
#	-f  "a/b"  \
#  -c "n/l" \
#  -e "in b, add element n/l" \
#  -t \
#  -p \
#  -c "a/c/x" \
#  -e "in a, add c/x" \
#  -t \
#  -p \
#  -e "serialize"\
#  -s \
#  -t \
#  -f "a" \
#  -p \
#  -d \
#  -m
