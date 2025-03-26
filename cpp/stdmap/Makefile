CC=g++ -Wall -ggdb3

testMap: testMap.cpp trimStr.cpp
	${CC} -fsanitize=leak -DMAPTEST $^ -pedantic -llsan -lpthread ${LINK} -o $@ 

trimStr: trimStr.cpp
	${CC} -fsanitize=leak -DTEST $^ -pedantic -llsan -lpthread ${LINK} -o $@ 

testMap2: testMap2.cpp trimStr.cpp commonClasses/BearerToken.cpp commonClasses/SiteTokens.cpp commonClasses/ReaderWriterLock.cpp
	${CC} -fsanitize=leak -DMAPTEST $^ -pedantic -llsan -lpthread ${LINK} -o $@ 


clean:
	rm -f testMap.o testMap trimStr
	
