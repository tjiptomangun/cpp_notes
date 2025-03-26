#!/bin/bash
./hexstrdump $1
xxd -c256 -g1 result.bin  | sed -e 's/^.*://g; s/^ //g; s/\s\s//g;s/\.\..*//g; s/ /, /g'

