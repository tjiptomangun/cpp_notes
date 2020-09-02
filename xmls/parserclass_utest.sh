#!/bin/bash 
./parserclass_test -i "<a><b foo=\"bar\" stat=\"camp\"></b></a>" \
  -p \
  -f "a" \
  -a "collar=blue"\
  -p \
  -d \
  -t \
  -p \
  -m
