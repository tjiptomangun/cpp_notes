#!/bin/bash
$CC -c -Wall -ggdb3 -I$ORACLE_HOME/rdbms/demo \
-I$ORACLE_HOME/rdbms/public \
-I$ORACLE_HOME/plsql/public \
-I$ORACLE_HOME/network/public \
-I$ORACLE_HOME/precomp/public committest.c
$CC -ggdb3 -Wall -L$ORACLE_HOME/lib/ -L$ORACLE_HOME/rdbms/lib/ -o committest \
committest.o -lclntsh -lpthread

