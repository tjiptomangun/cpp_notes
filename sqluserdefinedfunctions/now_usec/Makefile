#check with mysql
#SHOW VARIABLES LIKE 'plugin_dir';
#https://stackoverflow.com/questions/55241615/mysql-sys-exec-cant-open-shared-library-lib-mysqludf-sys-so-errno-11-wrong
LIBDIR=/usr/lib/mysql/plugin

install:
	gcc -g -ggdb -Wall -fPIC -rdynamic -I/usr/include/mysql -I. -shared  -o now_usec.so now_usec.c
	cp now_usec.so $(LIBDIR)/
