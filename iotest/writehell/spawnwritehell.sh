#!/bin/bash
eval logstart=$1
eval max=$2
eval n=0

while [ $n -lt $max ]; do
	./writehell.sh 2 2 100 $logstart	
	let logstart=logstart+1
	wait
	let n=n+1
done

