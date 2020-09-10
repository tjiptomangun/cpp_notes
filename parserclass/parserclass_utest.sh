#!/bin/bash 
./parserclass_test -c "a"\
  -e "add some b{"\
  -l "b" -t -f "a" -l "b" -t -f "a" -l"b"\
  -t -p\
  -e "}"\
  -e "add some d{"\
  -t -f "a" -l "d" -t -f "a" -l "d" -t -f"a" -l"d"\
  -t -p\
  -e "}"\
  -e "add some x{"\
  -t -f "a" -l "x" -t -f "a" -l "x"\
  -t -p\
  -e "}"\
  -e "sort ascending{"\
  -t -f "a"\
  -w "a" -t -p \
  -e "}"\
  -e "sort descending{"\
  -t -f "a"\
  -w "d" -t -p \
  -e "}"



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
#  
