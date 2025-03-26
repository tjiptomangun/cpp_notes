#!/bin/bash
sed '/^\s*$/d' $1 | sed '/^\*/d'| sed -e 's/SCCP_GTT_PATTERN/-t/g' | sed 's/SCCP_GTT_ADDRESS/-d/g' | sed -e 's/SCCP_GTT/-j/g' | awk '{print $1 " -i"$2 " -a"$3 " -p"$4 " -s"$5 " -g"$6 " -b"$7}'  | xargs -L1 ./hexstrtosiug5x | sed -e 's/,SEC_GTAID=0//g' | sed -e 's/$/;/g' | sed '/SCCP_GTT:GTTID=0/s/$/\n/g'

