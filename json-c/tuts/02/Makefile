CC=gcc -Wall -ggdb3
LINK=-L/usr/lib/x86_64-linux-gnu -ljson-c 
# https://stackoverflow.com/a/3220288
#json-version: json-version.c
#	gcc -Wall -ggdb3 json-version.c -L/usr/lib/x86_64-linux-gnu -ljson-c  -o json-version

all: json_new_empty json-new00 json-new01 json-new02 json-readplainhttpresponse01

json_new_empty: json_new_empty.c
	${CC} $^ ${LINK} -o $@  

json-new00: json-new00.c
	${CC} $^ ${LINK} -o $@  

json-new01: json-new01.c
	${CC} $^ ${LINK} -o $@  

json-new02: json-new02.c
	${CC} $^ ${LINK} -o $@ 
 
json-readplainhttpresponse01: json-readplainhttpresponse01.c
	${CC} $^ ${LINK} -o $@ 


clean:
	rm -f json_new_empty json-new00 json-new01 json-new02 json-readplainhttpresponse01
