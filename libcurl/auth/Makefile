CC=gcc -Wall -ggdb3
LINK=-L/usr/lib/x86_64-linux-gnu  -lcurl

all: basicauthpost basicauthget01 basicauthget02 basicauthget03 \
	basicbearerjson01 basicbearerjson02 basicbearerjson03 \
	basicbearerjson04 basicbearerjson05 basicbearerjson06 \
	basicbearerjson07 basicbearerjson07T withHttps getToken delDelete
  
basicauthpost: basicauthpost.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

basicauthget01: basicauthget01.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

basicauthget02: basicauthget02.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

basicauthget03: basicauthget03.c
	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

basicbearerjson01: basicbearerjson01.c
	${CC} -fsanitize=leak $^ -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson02: basicbearerjson02.c
	${CC}  $^ -lpthread -ljson-c ${LINK} -o $@ 

basicbearerjson03: basicbearerjson03.c
	${CC} -fsanitize=leak $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson04: basicbearerjson04.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson05: basicbearerjson05.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson06: basicbearerjson06.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson07: basicbearerjson07.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

basicbearerjson07T: basicbearerjson07.c
	${CC} -fsanitize=thread  $^ -lpthread -ljson-c -ltsan ${LINK} -o $@ 

withHttps: withHttps.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

delDelete: delDelete.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 

getToken: getToken.c
	${CC} -fsanitize=leak  $^ -lpthread -ljson-c -llsan ${LINK} -o $@ 
# basicbearerjson04: basicbearerjson04.c
# 	${CC} $^ -lpthread -ljson-c ${LINK} -o $@ 


#iterateOption: iterateOption.c
#	${CC} -fsanitize=leak $^ -llsan ${LINK} -o $@ 

clean:
	rm -f basicauthpost basicauthget01 basicauthget02 basicauthget03 \
	basicbearerjson01 basicbearerjson02 basicbearerjson03 \
	basicbearerjson04 basicbearerjson05 basicbearerjson06 \
	basicbearerjson07 basicbearerjson07T withHttps getToken delDelete