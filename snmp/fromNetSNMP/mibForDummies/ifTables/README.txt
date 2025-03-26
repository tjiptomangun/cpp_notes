http://www.net-snmp.org/wiki/index.php/MFD:ifTable
run all the three mibc2c -c command

https://sourceforge.net/p/net-snmp/bugs/2598/#7ffb
may need to comment content of ifTable_container_load
may need to add -fPIC  in Makefile
run with
./ifTable -f -L -DifTable -M localhost:1161
check with
sudo netstat -vatnup |grep ifTable
or 
lsof -i udp:1161

https://net-snmp.sourceforge.io/tutorial/tutorial-5/toolkit/mfd/if-mib/ifTable/source/ifTable_data_access.c
mfd is mib for dummies



