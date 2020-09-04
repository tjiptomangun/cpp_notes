#!/bin/bash 
./parserclass_test -i "<a x=\"y\" ><b></b></a>" \
	-f "a"\
	-t \
	-p \
	-f "a" \
	-r "x" \
	-p \
	-d \
	-t \
	-p \
	-m 
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
