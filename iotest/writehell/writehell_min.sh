#!/bin/bash
eval max=$1
eval start=$(($(date +%s%N)/1000000))

./writehell log/hei00.log  $max & 

wait 
eval end=$(($(date +%s%N)/1000000))
let delta=$end-$start
echo $delta milliseconds

