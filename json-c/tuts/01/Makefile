CC=gcc -Wall -ggdb3
LINK=-L/usr/lib/x86_64-linux-gnu -ljson-c 
# https://stackoverflow.com/a/3220288
#json-version: json-version.c
#	gcc -Wall -ggdb3 json-version.c -L/usr/lib/x86_64-linux-gnu -ljson-c  -o json-version

all: json-version json-file00 json-file01 \
	json-parse00 json-parse01 json-parse02 \
	json-type00 json-parse04 json-str00 \
	json-parse05 json-parse06 json-parse07 \
	json-parse08

json-version: json-version.c
	${CC} $^ ${LINK} -o $@  

json-file00: json-file00.c
	${CC} $^ ${LINK} -o $@  

json-file01: json-file01.c
	${CC} $^ ${LINK} -o $@  

json-parse00: json-parse00.c
	${CC} $^ ${LINK} -o $@  

json-parse01: json-parse01.c
	${CC} $^ ${LINK} -o $@  

json-parse02: json-parse02.c
	${CC} $^ ${LINK} -o $@  

json-type00: json-type00.c
	${CC} $^ ${LINK} -o $@  

json-parse04: json-parse04.c
	${CC} $^ ${LINK} -o $@  

json-str00: json-str00.c
	${CC} $^ ${LINK} -o $@  

json-parse05: json-parse05.c
	${CC} $^ ${LINK} -o $@  

json-parse06: json-parse06.c
	${CC} $^ ${LINK} -o $@  

json-parse07: json-parse07.c
	${CC} $^ ${LINK} -o $@  

json-parse08: json-parse08.c
	${CC} $^ ${LINK} -o $@  


clean:
	rm -f json-version json-file00 json-file01 \
	json-parse00 json-parse01 json-parse02 \
	json-type00 json-parse04 json-str00 \
	json-parse05 json-parse06 json-parse07 \
	json-parse08
