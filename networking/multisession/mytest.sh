#!/usr/local/bin/bash
pids_pc=`ps aux|grep EchoSvtr | grep -v grep | sed -e 's/^[^ ][^ ]*  *\([0-9][0-9]*\).*/\1/g'`
num_pc=${#pids_pc[@]}
n=0

for p in $pids_pc
do
	echo "XLink already exists " $p
	(( n += 1)) 
done

pids_pc=`ps aux|grep EchoSvrk | grep -v grep | sed -e 's/^[^ ][^ ]*  *\([0-9][0-9]*\).*/\1/g'`
num_pc=${#pids_pc[@]}

for p in $pids_pc
do
	echo "PC already exists " $p
	(( n += 1)) 
done


if [ $n -gt 0 ]
then
	exit 1  
fi


echo $$
